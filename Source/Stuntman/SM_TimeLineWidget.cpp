// Fill out your copyright notice in the Description page of Project Settings.

#include "SM_TimeLineWidget.h"
#include "Math/Color.h"

int32 USM_TimeLineWidget::NativePaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const
{
    Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

/*// draw a rectangle in UMG unreal 5.4
FVector2D RectPosition(50, 50);
FVector2D RectSize(100, 100);
//FPaintGeometry RectGeometry(RectPosition, RectSize);
FPaintGeometry RectGeometry = AllottedGeometry.ToPaintGeometry(RectPosition, RectSize);
FLinearColor RectColor(0, 0, 1, 1);
FSlateDrawElement::MakeBox(
    OutDrawElements,
    LayerId,
    RectGeometry,
    nullptr,
    ESlateDrawEffect::None,
    RectColor
);
*/
/*FSlateDrawElement::MakeBox(
    OutDrawElements,
    LayerId,
    RectGeometry,
    FSlateBrush(RectColor),    
    ESlateDrawEffect::None,
    RectColor
);*/


    /*FLinearColor Color = FLinearColor(1, 0, 0, 1);
    FPaintedLine Line = FPaintedLine(FVector2D(0, 0), FVector2D(100, 100), Color);
    Line.Draw(OutDrawElements, LayerId, AllottedGeometry);

    FLinearColor Color2 = FLinearColor(0, 1, 0, 1);
    FPaintedLine Line2 = FPaintedLine(FVector2D(0, 100), FVector2D(100, 0), Color2);
    Line2.Draw(OutDrawElements, LayerId, AllottedGeometry);*/

    return LayerId;
}