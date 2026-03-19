
#include "util/pcgsolver.h"
#include "solver.hpp"

void ExplicitSolver::Step(Grid2D& grid, float stepTime, float diffusivity) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();
    const int32_t size = dimX * dimY;

    std::vector<float> values(size);
    int32_t index = 0;

    for (int32_t y = 0; y < dimY; ++y)
    { 
        for (int32_t x = 0; x < dimX; ++x)
        {
            const float delta = stepTime * diffusivity * grid.GetLaplacian(x, y);
            values[index++] = grid.Get(x, y) + delta;
        }
    }
    grid.SetAll(values);
}

void ExplicitSolver::Step(Grid3D& grid, float stepTime, float diffusivity) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();
    const int32_t dimZ = grid.GetDimZ();
    const int32_t size = dimX * dimY * dimZ;

    std::vector<float> values(size);
    int32_t index = 0;

    for (int32_t z = 0; z < grid.GetDimZ(); ++z)
    {
        for (int32_t y = 0; y < grid.GetDimY(); ++y)
        { 
            for (int32_t x = 0; x < grid.GetDimX(); ++x)
            {
                const float delta = stepTime * diffusivity * grid.GetLaplacian(x, y, z);
                values[index++] = grid.Get(x, y, z) + delta;
            }
        }
    }
    grid.SetAll(values);
}

void ImplicitSolver::Step(Grid2D& grid, float stepTime, float diffusivity) const noexcept
{ 
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();
    const int32_t size = dimX * dimY;

    const float stepX = grid.GetStepX();
    const float stepY = grid.GetStepY();

    const float lambdaX = stepTime * diffusivity / (stepX * stepX);
    const float lambdaY = stepTime * diffusivity / (stepY * stepY);

    SparsePCGSolver<float> pcg;
    SparseMatrix<float> matrix(size, 5);
   
    // Fill the sparse matrix
    for (int32_t i = 0; i < size; ++i)
    {
        const int32_t x = i % dimX;
        const int32_t y = i / dimX;

        if (y > 0)
            matrix.set_element(i, i-dimX, -lambdaY);
        if (x > 0)
            matrix.set_element(i, i-1, -lambdaX);

        matrix.set_element(i, i, 1.0f + 2.0f * (lambdaX + lambdaY));
        
        if (x < dimX-1)
            matrix.set_element(i, i+1, -lambdaX);
        if (y < dimY-1)
            matrix.set_element(i, i+dimX, -lambdaY);
    }

    const std::vector<float>& rhs = grid.GetAll();
    std::vector<float> result(size);
    float relativeResidual;
    int32_t iterations;

    bool success = pcg.solve(matrix, rhs, result, relativeResidual, iterations);
    if (success)
        grid.SetAll(result);
}

void ImplicitSolver::Step(Grid3D& grid, float stepTime, float diffusivity) const noexcept
{
    const int32_t dimX = grid.GetDimX();
    const int32_t dimY = grid.GetDimY();
    const int32_t dimZ = grid.GetDimZ();
    const int32_t size = dimX * dimY * dimZ;

    const float stepX = grid.GetStepX();
    const float stepY = grid.GetStepY();
    const float stepZ = grid.GetStepZ();

    const float lambdaX = stepTime * diffusivity / (stepX * stepX);
    const float lambdaY = stepTime * diffusivity / (stepY * stepY);
    const float lambdaZ = stepTime * diffusivity / (stepZ * stepZ);

    SparsePCGSolver<float> pcg;
    SparseMatrix<float> matrix(size, 7);
   
    // Fill the sparse matrix
    for (int32_t i = 0; i < size; ++i)
    {
        const int32_t x = i % dimX;
        const int32_t y = (i / dimX) % dimY;
        const int32_t z = i / (dimX * dimY);

        if (z > 0)
            matrix.set_element(i, i-dimX*dimY, -lambdaZ);
        if (y > 0)
            matrix.set_element(i, i-dimX, -lambdaY);
        if (x > 0)
            matrix.set_element(i, i-1, -lambdaX);

        matrix.set_element(i, i, 1.0f + 2.0f * (lambdaX + lambdaY + lambdaZ));
        
        if (x < dimX-1)
            matrix.set_element(i, i+1, -lambdaX);
        if (y < dimY-1)
            matrix.set_element(i, i+dimX, -lambdaY);
        if (z < dimZ-1)
            matrix.set_element(i, i+dimX*dimY, -lambdaZ);
    }

    const std::vector<float>& rhs = grid.GetAll();
    std::vector<float> result(size);
    float relativeResidual;
    int32_t iterations;

    bool success = pcg.solve(matrix, rhs, result, relativeResidual, iterations);
    if (success)
        grid.SetAll(result);
}

