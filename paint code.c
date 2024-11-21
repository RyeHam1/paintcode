float3 mean[4] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

float3 sigma[4] = {
    {0, 0, 0}, 
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

float2 offsets[4] = {
    {-Radius, -Radius}, 
    {-Radius, 0}, 
    {0, -Radius}, 
    {0, 0}
};

float2 pos;
float3 col;

// Accumulate colors for each region
for (int i = 0; i < 4; i++) 
{
    // Reset color and squared color for each region
    for (int j = 0; j <= Radius; j++) 
    {
        for (int k = 0; k <= Radius; k++) 
        {
            pos = float2(j, k) + offsets[i];

            // Calculate the UV position normalized by the ViewSize
            float2 uvpos = (UV + pos / ViewSize);
            uvpos = clamp(uvpos, float2(0.0, 0.0), float2(1.0, 1.0));  // Ensure UV is valid

            // Sample the color at the current UV position
            col = SceneTextureLookup(uvpos, 14, false);

            // Gets mean and squared values for variance calculation
            mean[i] += col;
            sigma[i] += col * col;
        }
    }
}

// Number of samples per region
float n = pow(Radius + 1, 2);

float minSigma = 1e10; // A very large value to start with
float3 resultColor = float3(0, 0, 0);

// Calculate mean, variance, and find the region with the smallest variance
for (int i = 0; i < 4; i++) 
{
    // Normalize the gets color and squared color
    mean[i] /= n;
    sigma[i] = abs(sigma[i] / n - mean[i] * mean[i]); 

    
    float sigmaTotal = sigma[i].r + sigma[i].g + sigma[i].b;

    // Choose the region with the smallest variance
    if (sigmaTotal < minSigma) 
    {
        minSigma = sigmaTotal;
        resultColor = mean[i];
    }
}

// Return the color
return resultColor;