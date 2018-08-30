
#include "Utlities.h"

#include "Items/Sword/BladePart.h"
#include "Items/Sword/HandlePart.h"

ESwordPart UUtilities::GetSwordPartEnum(AForgePart * Part)
{
	if (ABladePart* Blade = Cast<ABladePart>(Part))
	{
		if (Blade->BladeMat == EBladeMat::BM_IRON)
		{
			if (Blade->BladeType == EBladeType::BT_BROADSWORD)
			{
				return ESwordPart::PT_IRONBROADSWORDBLADE;
			}
			else
			{
				return ESwordPart::PT_IRONKRISBLADE;
			}
		}
		else if (Blade->BladeMat == EBladeMat::BM_STEEL)
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
		if (Blade->HandleType == EHandleType::HT_BROADSWORD)
		{
			return ESwordPart::PT_BROADSWORDHANDLE;
		}
		else
		{
			return ESwordPart::PT_KRISHANDLE;
		}
	}
	return ESwordPart();
}

