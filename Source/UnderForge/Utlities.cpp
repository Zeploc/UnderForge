
#include "Utlities.h"

#include "Items/Sword/BladePart.h"
#include "Items/Sword/HandlePart.h"

ESwordPart UUtilities::GetSwordPartEnum(AForgePart * Part)
{
	if (ABladePart* Blade = Cast<ABladePart>(Part))
	{
		if (Blade->BladeMat == EBladeMat::BM_BRONZE)
		{
			if (Blade->BladeType == EBladeType::BT_BROADSWORD)
			{
				return ESwordPart::PT_BRONZEBROADSWORDBLADE;
			}
			else
			{
				return ESwordPart::PT_BRONZEKRISBLADE;
			}
		}
		else if (Blade->BladeMat == EBladeMat::BM_IRON)
		{
			if (Blade->BladeType == EBladeType::BT_BROADSWORD)
			{
				return ESwordPart::PT_STEELBROADSWORDBLADE;
			}
			else
			{
				return ESwordPart::PT_STEELKRISBLADE;
			}
		}
	}
	else if (AHandlePart* Blade = Cast<AHandlePart>(Part))
	{
		if (Blade->HandleType == EHandleType::HT_CURVED)
		{
			return ESwordPart::PT_CURVEDHANDLE;
		}
		else
		{
			return ESwordPart::PT_STRAIGHTHANDLE;
		}
	}
	return ESwordPart();
}

