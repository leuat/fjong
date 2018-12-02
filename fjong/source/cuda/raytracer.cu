
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
__device__ bool raymarchSingle(ray& r, int pass, int cnt, World* world, marchobject* objects)//, hitable **world) {
{
     vec3 isp;
     float shadow = 1;
     float t = 1;
     int winner = -1;
     r.curPos = r.org;
    world->length=7;
        for (int i=0;i<cnt;i++) {
            float precis = 0.004*t;
            float keep=1000.0;
            //ray.m_curStep =t; //(ray.m_origin-m_objects[j]->m_position).length();
            //ray.setCurrent(t);
            r.curPos = r.point_at_parameter(t);

            int w= -1;
            for (int j=0;j<world->length;j++) {//marchobject* ro: culled) {
//                marchobject* ro = world->objects[i];
//                if (ro==ignore)
  //                  continue;
                //ro->m_localRay[tid].setCurrent(t);
                float keep2=1000;
                if (objects[j].type==0)
                    keep2 = marchobject::intersectSphere(r, objects[j].pos, objects[j].p1.x());

//                float keep2 =world->objects[j].intersect(r);
                if (keep2<keep) {
                    keep = keep2;
                    w = j;

                }

                if (keep2<precis) {
                    winner = w;
                    i=cnt;
                    if (pass==2)
                        return true;
                    break;

                }
            }
                t=t+keep;

        }



        if (winner!=-1) {
            //Ray rotated = winner->m_localRay[tid];//ray.Rotate(winner->m_rotmat, winner->m_position);
             ray rotated = r;
            //ray.m_currentPos = isp;
    //                exit(1);
            isp = rotated.curPos;
            r.intensity = vec3(1,0,0);
/*__global__ void create_world(hitable **d_list, hitable **d_world, camera **d_camera) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        d_list[0] = new sphere(vec3(0,0,-1), 0.5,
                               new lambertian(vec3(0.8, 0.3, 0.3)));
        d_list[1] = new sphere(vec3(0,-100.5,-1), 100,
                               new lambertian(vec3(0.8, 0.8, 0.0)));
        d_list[2] = new sphere(vec3(1,0,-1), 0.5,
                               new metal(vec3(0.8, 0.6, 0.2), 1.0));
        d_list[3] = new sphere(vec3(-1,0,-1), 0.5,
                               new metal(vec3(0.8, 0.8, 0.8), 0.3));
        *d_world  = new hitable_list(d_list,4);
        *d_camera = new camera();
    }
}            vec3 normal = winner->CalcMarchNormal(rotated.m_currentPos);
            //normal = winner->m_rotmatInv*normal;
            vec3 tt(1,2,-213.123);
            vec3 tangent = vec3::crossProduct(tt,normal).normalized();
            vec3 bi = vec3::crossProduct(tangent,normal).normalized();

            //normal  = winner->GetPerturbedNormal(isp,normal,tangent,m_globals);


    //        ray.m_reflect = 0;
            QVector3D reflectionDir = ray.m_direction-2*QVector3D::dotProduct(ray.m_direction, normal)*normal;
            QVector3D lp = ray.m_currentPos;//-winner->m_localPos;
     //       ray.m_z=10000;
            winner->CalculateLight(&ray,normal,tangent,lp,m_globals,reflectionDir,m_objects,0);


            if (winner->m_material.m_reflectivity>0 && ray.m_reflect>0) {
                Ray nxt(lp,reflectionDir);
                nxt.m_reflect=ray.m_reflect-1;
                RayMarchSingle(nxt, Reflect, winner,24,tid);

                ray.m_intensity = ray.m_intensity*(1-winner->m_material.m_reflectivity) + winner->m_material.m_reflectivity*nxt.m_intensity;
            }

    //        if (pass==Image)

            for (AbstractLight* al: m_globals.m_lights) {
    //            Ray shadowRay(isp,winner->m_rotmat*al->m_direction*1);
                Ray shadowRay(lp,al->m_direction);
                AbstractRayObject* o= nullptr;
                if (dynamic_cast<RayObjectBox*>(winner)!=nullptr)
                    o=winner;
                if (RayMarchSingle(shadowRay, Shadow, o,14,tid)) {
                    shadow*=0.5;
                }

            }

            ray.m_intensity*=shadow;

            */
            return true;

        }
//        m_globals.Sky(&ray,m_globals.m_skyScale);

        return false;
    }




__global__ void renderImage(vec3 *fb, int max_x, int max_y,vec3 lower_left_corner, vec3 horizontal, vec3 vertical, vec3 origin, World *world, marchobject* objects)
{
   int i = threadIdx.x + blockIdx.x * blockDim.x;
   int j = threadIdx.y + blockIdx.y * blockDim.y;
   if((i >= max_x) || (j >= max_y)) return;
   int pixel_index = j*max_x + i;
   float u = float(i) / float(max_x);
   float v = float(j) / float(max_y);
   ray r(origin, (lower_left_corner + u*horizontal + v*vertical).normalized());
   r.reflect = 3;
   vec3 col(0,0,0);
//   fb[pixel_index] = color(r, world);
   if (raymarchSingle(r,0,64,world, objects)) {
       col = r.intensity;
   }
   fb[pixel_index] = col;

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



void RaytraceImage(int nx, int ny, int* img, World* w) {
    int num_pixels = nx*ny;

    size_t fb_size = num_pixels*sizeof(vec3);

    // allocate FB
    vec3 *fb;
    checkCudaErrors(cudaMallocManaged((void **)&fb, fb_size));
    World* world;
    int bytesw = (sizeof(World));
    int bytesm = (w->length*(sizeof(marchobject)));
    marchobject* objects;
    checkCudaErrors(cudaMallocManaged((void **)&world, bytesw));
    checkCudaErrors(cudaMallocManaged((void **)&objects, bytesm));
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
    //renderImage<<<blocks, threads>>>(fb, nx, ny);
    renderImage<<<blocks, threads>>>(fb, nx, ny,
                                    vec3(-2.0, -1.0, -1.0),
                                    vec3(4.0, 0.0, 0.0),
                                    vec3(0.0, 2.0, 0.0),
                                    vec3(0.0, 0.0, 0.0), world, objects);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());




    for (int j = ny-1; j >= 0; j--) {
           for (int i = 0; i < nx; i++) {
               size_t pixel_index = j*nx + i;
               vec3& c = fb[pixel_index + 0];
               int ir = int(255.99*c.x());
               int ig = int(255.99*c.y());
               int ib = int(255.99*c.z());

               img[3*pixel_index+0] = ir;
               img[3*pixel_index+1] = ig;
               img[3*pixel_index+2] = ib;

               //std::cout << ir << " " << ig << " " << ib << "\n";
           }
       }
       checkCudaErrors(cudaFree(fb));
       checkCudaErrors(cudaFree(objects));
       checkCudaErrors(cudaFree(world));


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
