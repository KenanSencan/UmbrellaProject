# Diffuse light 
- There's `SurfaceNormal`: `{0.f, -1.f}` it's pointing **upwards**. it's represented with the green line 
- intensity (yellow line) calculated as `Math::Dot(SurfaceNormal, lightDir)`

# Key catch 
- the `lightDir` calculated just as normal 
- The the intensity will increase if the `lightDir` is close to `SurfaceNormal` and decrease if the `lightDir` is far from `SurfaceNormal`

So the calculated direction needs to align with the normal. 