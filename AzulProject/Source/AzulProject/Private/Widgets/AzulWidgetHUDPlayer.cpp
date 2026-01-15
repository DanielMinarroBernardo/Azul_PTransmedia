#include "Widgets/AzulWidgetHUDPlayer.h"

void UAzulWidgetHUDPlayer::SetUIState(EInteractUIState NewState)
{
    if (!InteractImage || !CrosshairTexture)
        return;

    InteractImage->SetBrushFromTexture(CrosshairTexture, true);

    switch (NewState)
    {
    case EInteractUIState::Default:
        InteractImage->SetColorAndOpacity(DefaultColor);
        break;

    case EInteractUIState::Active:
        InteractImage->SetColorAndOpacity(ActiveColor);
        break;
    }
}
