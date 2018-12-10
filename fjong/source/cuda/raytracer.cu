
#include <stdio.h>
#include <iostream>
// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>
#include "vec3.h"
#include "ray.h"
#include "marchobject.h"
#include "world.h"

#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
    if (result) {
        std::cout << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
        file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}



/**
 * CUDA Kernel Device code
 *
 * Computes the vector addition of A and B into C. The 3 vectors have the same
 * number of elements numElements.
 */
__global__ void
vectorAdd(const float *A, const float *B, float *C, int numElements)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < numElements)
    {
        C[i] = A[i] + B[i];
    }
}


/**
 * Host main routine
 */



//__device__ bool raymarchSingle(ray& r, int ignore, int pass, int cnt, World* world, marchobject** objects, int* culled, int tid)//, hitable **world) {
__device__ bool raymarchSingle(ray& r, int ignore, int pass, int cnt, World* world, marchobject** objects)//, hitable **world) {
{
    //marchobject::Init();
     vec3 isp;
     float shadow = 1;
     float t = 1;
     int winner = -1;

     int culled[24];
//     ray culledr[16];


     int len = 0;

     vec3 isp1, isp2;
     float t1, t2;
     for (int i=0;i<world->length;i++) {
         if (len>=24) break;
         if (i!=ignore)
         if (r.IntersectSphere(objects[i]->pos*-1,vec3(1,1,1)*objects[i]->bbRadius,isp1,isp2,t1,t2)) {
             culled[len] = i;
             len++;
         }
     }
     ray rwinner;
     r.curPos = r.org;
     //   world->length=7;
     for (int i=0;i<cnt;i++) {
         float precis = 0.004*t;
         float keep=1000.0;
         //ray.m_curStep =t; //(ray.m_origin-m_objects[j]->m_position).length();
         //ray.setCurrent(t);
         r.curPos = r.point_at_parameter(t);
        // r.point_at_parameter(t);
         int w= -1;
         //            for (int j=0;j<world->length;j++) {//marchobject* ro: culled) {
         for (int l=0;l<len;l++) {//marchobject* ro: culled) {
             int j = culled[l];

             ray rotr = r.Rotate(objects[j]->rotMat,objects[j]->pos);
             float keep2 = objects[j]->intersect(rotr);
             if (keep2<keep) {
                 keep = keep2;
                 w = j;


             }

             if (keep2<precis) {
                 winner = w;
                 i=cnt;
                 if (pass==2) {

                     return true;
                 }
                 rwinner = rotr;
                 break;

             }
         }
         t=t+keep;

     }


     if (winner!=-1) {
            //Ray rotated = winner->m_localRay[tid];//ray.Rotate(winner->m_rotmat, winner->m_position);
             ray rotated = rwinner;
            //ray.m_currentPos = isp;
    //                exit(1);
            isp = rotated.curPos;
            r.intensity = vec3(1,0,0);
            vec3 normal = objects[winner]->CalcMarchNormal(rotated.curPos);
            normal = objects[winner]->rotMatInv*normal;
            vec3 tt(1,2,-213.123);
            vec3 tangent =vec3::cross(tt,normal).normalized();
            vec3 bi = vec3::cross(tangent,normal).normalized();

//            normal  = objects[winner]->GetPerturbedNormal(isp,normal,tangent);


    //        ray.m_reflect = 0;
            vec3 reflectionDir = r.dir-normal*2*dot(r.dir, normal);;
            vec3 lp = r.curPos;//-winner->m_localPos;
     //       ray.m_z=10000;
            objects[winner]->CalculateLight(&r,normal,tangent,lp,world->light0,reflectionDir,objects,0);

//            objects[winner]->reflectivity = 0.9;

            // Reflections
            if (objects[winner]->reflectivity>0 && r.reflect>0) {
                if (objects[winner]->glossiness==1)
                {
                    ray nxt(lp,reflectionDir);
                    nxt.reflect=r.reflect-1;
                    raymarchSingle(nxt, winner, 1, 24,world, objects);
                    r.intensity = r.intensity*(1-objects[winner]->reflectivity) + objects[winner]->reflectivity*nxt.intensity;
                }
                else {
                    // Doesn't work
                    shadow=1;
                    len=0;
                    vec3 in = vec3(0,0,0);
                    for (int j=0;j<shadow;j++) {
                        vec3 disp = vec3(world->rnd[3*j+0]%1024-512,world->rnd[3*j+1]%1024-512,world->rnd[3*j+2]%1024-512).normalized();
                        disp = (disp*3 + reflectionDir.normalized()).normalized();
                        ray nxt(lp,disp);
                        nxt.reflect=0;
                        raymarchSingle(nxt, winner, 1, 24,world, objects);
                        in+=nxt.intensity/(float)shadow;
                    }
                    r.intensity = r.intensity*(1-objects[winner]->reflectivity) + objects[winner]->reflectivity*in;
                   }
                    shadow = 1;
            }
            if (pass==0) {
                ray shadowRay(lp,world->light0);
                if (raymarchSingle(shadowRay, winner, 2,32,world,objects)) {
                    shadow*=0.5;
                }

            }

            r.intensity*=shadow;

            return true;

        }

       world->sky(r);

        return false;
    }

__global__ void create_world(marchobject* objects, marchobject** objectsI, int cnt) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        for (int i=0;i<cnt;i++) {
            objectsI[i] = nullptr;
            if (objects[i].type==0) {
                mo_sphere* s = new mo_sphere();
                *(objectsI+i) = s;
                *s =  (mo_sphere&)objects[i];
            }
            if (objects[i].type==1) {
                mo_plane* s = new mo_plane();
                *(objectsI+i) = s;

                *s =  (mo_plane&)objects[i];
//                s->pos = vec3(0,4,0);
            }
            if (objects[i].type==2) {
                mo_box* s = new mo_box();
                *(objectsI+i) = s;
                *s =  (mo_box&)objects[i];
                s->box = objects[i].p2;
//                s->pos = vec3(0,4,0);
            }
            if (objects[i].type==3) {
                mo_torus* s = new mo_torus();
                *(objectsI+i) = s;
                *s = (mo_torus&)objects[i];
//                s->pos = vec3(0,4,0);
            }
            if (objects[i].type==4) {
                mo_cylinder* s = new mo_cylinder();
                *(objectsI+i) = s;
                *s = (mo_cylinder&)objects[i];
//                s->pos = vec3(0,4,0);
            }
        }

/*        *(d_list)   = new sphere(vec3(0,0,-1), 0.5);
        *(d_list+1) = new sphere(vec3(0,-100.5,-1), 100);
        *d_world    = new hitable_list(d_list,2);*/
    }
}

__global__ void delete_world(marchobject* objects, marchobject** objectsI, int cnt) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        for (int i=0;i<cnt;i++) {
            if (objectsI[i]!=nullptr)
                delete objectsI[i];
        }
    }

}


__global__ void renderImage(unsigned char *fb, int max_x, int max_y,vec3 lower_left_corner, vec3 horizontal, vec3 vertical, vec3 origin, World *world, marchobject** objects)
{
   int i = threadIdx.x + blockIdx.x * blockDim.x;
   int j = threadIdx.y + blockIdx.y * blockDim.y;
   if((i >= max_x) || (j >= max_y)) return;
   int pixel_index = j*max_x + i;
   float u = float(i) / float(max_x);
   float v = float(j) / float(max_y);
   ray r(origin, (lower_left_corner + u*horizontal + v*vertical).normalized());
//   ray r(origin, (u*horizontal + v*vertical).normalized());
   r.reflect = 2;
//   vec3 col(0,0,0);
//   fb[pixel_index] = color(r, world);
   r.intensity = vec3(0,0,0);
   if (raymarchSingle(r,-1,0,90,world, objects)) {
  //     col = r.intensity;
   }
   vec3 in = r.intensity.clamp();
   fb[3*pixel_index] = in.x()*255;
   fb[3*pixel_index+1] = in.y()*255;
   fb[3*pixel_index+2] = in.z()*255;

//   raymarchSingle(const ray& r, int pass, int cnt, World* world)//, hitable **world)
}


/*__global__ void renderImage(float *fb, int max_x, int max_y) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if((i >= max_x) || (j >= max_y)) return;
    int pixel_index = j*max_x*3 + i*3;
    fb[pixel_index + 0] = float(i) / max_x;
    fb[pixel_index + 1] = float(j) / max_y;
    fb[pixel_index + 2] = 0.2;
}
*/


unsigned char *fb = nullptr;

unsigned char* RaytraceImage(int nx, int ny, int* img, World* w) {
    int num_pixels = nx*ny;

    size_t fb_size = num_pixels*3;

    // allocate FB
    if (fb==nullptr)
        checkCudaErrors(cudaMallocManaged((void **)&fb, fb_size));

    World* world;
    int bytesw = (sizeof(World));
    int bytesm = (w->length*(sizeof(marchobject)));
    marchobject* objects;
    marchobject** objectsI;
    checkCudaErrors(cudaMallocManaged((void **)&world, bytesw));
    checkCudaErrors(cudaMallocManaged((void **)&objects, bytesm));
    checkCudaErrors(cudaMallocManaged((void **)&objectsI, bytesm));


/*    world->length = w->length;
    for (int i=0;i<w->length;i++)
        world->objects[i] = w->objects[i];*/
    cudaMemcpy(world,w,bytesw,cudaMemcpyHostToDevice);
    checkCudaErrors(cudaGetLastError());


    cudaMemcpy(objects, w->objects, bytesm, cudaMemcpyHostToDevice);
    checkCudaErrors(cudaGetLastError());
    int tx = 8;
    int ty = 8;
    dim3 blocks(nx/tx+1,ny/ty+1);
    dim3 threads(tx,ty);




    create_world<<<1,1>>>(objects,objectsI, world->length);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());


    renderImage<<<blocks, threads>>>(fb, nx, ny,
                                    w->lower_left_corner,
                                    w->horizontal,
                                    w->vertical,
                                    w->origin, world, objectsI);

    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    delete_world<<<1,1>>>(objects,objectsI, world->length);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());


       checkCudaErrors(cudaFree(objects));
       checkCudaErrors(cudaFree(world));

       return fb;

}



void TestCuda()
{
    // Error code to check return values for CUDA calls
    cudaError_t err = cudaSuccess;

    // Print the vector length to be used, and compute its size
    int numElements = 50000;
    size_t size = numElements * sizeof(float);
    printf("[Vector addition of %d elements]\n", numElements);

    // Allocate the host input vector A
    float *h_A = (float *)malloc(size);

    // Allocate the host input vector B
    float *h_B = (float *)malloc(size);

    // Allocate the host output vector C
    float *h_C = (float *)malloc(size);

    // Verify that allocations succeeded
    if (h_A == NULL || h_B == NULL || h_C == NULL)
    {
        fprintf(stderr, "Failed to allocate host vectors!\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the host input vectors
    for (int i = 0; i < numElements; ++i)
    {
        h_A[i] = rand()/(float)RAND_MAX;
        h_B[i] = rand()/(float)RAND_MAX;
    }

    // Allocate the device input vector A
    float *d_A = NULL;
    err = cudaMalloc((void **)&d_A, size);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Allocate the device input vector B
    float *d_B = NULL;
    err = cudaMalloc((void **)&d_B, size);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector B (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Allocate the device output vector C
    float *d_C = NULL;
    err = cudaMalloc((void **)&d_C, size);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector C (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copy the host input vectors A and B in host memory to the device input vectors in
    // device memory
    printf("Copy input data from the host memory to the CUDA device\n");
    err = cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector A from host to device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector B from host to device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Launch the Vector Add CUDA Kernel
    int threadsPerBlock = 256;
    int blocksPerGrid =(numElements + threadsPerBlock - 1) / threadsPerBlock;
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, numElements);
    err = cudaGetLastError();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copy the device result vector in device memory to the host result vector
    // in host memory.
    printf("Copy output data from the CUDA device to the host memory\n");
    err = cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector C from device to host (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Verify that the result vector is correct
    for (int i = 0; i < numElements; ++i)
    {
        if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5)
        {
            fprintf(stderr, "Result verification failed at element %d!\n", i);
            exit(EXIT_FAILURE);
        }
    }
    printf("Test PASSED\n");

    // Free device global memory
    err = cudaFree(d_A);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to free device vector A (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    err = cudaFree(d_B);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to free device vector B (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    err = cudaFree(d_C);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to free device vector C (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Free host memory
    free(h_A);
    free(h_B);
    free(h_C);

    // Reset the device and exit
    err = cudaDeviceReset();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to deinitialize the device! error=%s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    printf("Done\n");

    system("pause");

}
