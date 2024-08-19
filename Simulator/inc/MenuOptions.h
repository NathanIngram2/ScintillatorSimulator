#pragma once

enum MenuOptions
{
    DisplayConfiguration,
    SetNumOfPhotons,
    SetScintillatorMaterial,
    SetScatteringLength,
    SetFiberMaterial,
    SetFiberSpacing,
    SetFiberDiameter,
    AnimateDemo, // demo with the current config and a few photons (python)
    ExecuteMC,
    END // use this as loop teminating condition, nothing else
};