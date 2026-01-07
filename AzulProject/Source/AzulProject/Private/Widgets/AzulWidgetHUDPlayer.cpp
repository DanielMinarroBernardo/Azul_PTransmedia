#include "Widgets/AzulWidgetHUDPlayer.h"


void UAzulWidgetHUDPlayer::SetUIState(EInteractUIState NewState)
{
    if (!InteractImage) return;

    UTexture2D* TextureToApply = nullptr;

    switch (NewState)
    {
    case EInteractUIState::Default:
        TextureToApply = DefaultTexture;
        break;

    case EInteractUIState::InRange:
        TextureToApply = InRangeTexture;
        break;

    case EInteractUIState::InRangeAndLooking:
        TextureToApply = LookingTexture;
        break;
    }


    if (TextureToApply)
    {
        InteractImage->SetBrushFromTexture(TextureToApply, true);
    }

}