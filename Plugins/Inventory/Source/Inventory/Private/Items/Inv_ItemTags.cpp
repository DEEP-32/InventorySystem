#include <Items/Inv_ItemTags.h>



namespace GameItems {
	namespace Equipments {
		namespace Weapons {
			UE_DEFINE_GAMEPLAY_TAG( Axe, "GameItems.Equipments.Weapons.Axe" );
			UE_DEFINE_GAMEPLAY_TAG( Swords,"GameItems.Equipments.Weapons.Sword" );
			
		}

		namespace Cloaks {
			UE_DEFINE_GAMEPLAY_TAG( RedCloak,"GameItems.Equipments.Cloaks.RedCloak" );
			
		}

		namespace Masks {
			UE_DEFINE_GAMEPLAY_TAG( SteelMasks,"GameItems.Equipments.Masks.SteelMasks" );
		}
		
	}

	namespace Consumables {
		namespace Potions {
			UE_DEFINE_GAMEPLAY_TAG( RedSmallPotion, "GameItems.Consumables.Potions.RedSmallPotion" );
			UE_DEFINE_GAMEPLAY_TAG( RedLargePotion , "GameItems.Consumables.Potions.RedLargePotion");

			UE_DEFINE_GAMEPLAY_TAG( BlueSmallPotion ,"GameItems.Consumables.Potions.BlueSmallPotion");
			UE_DEFINE_GAMEPLAY_TAG( BlueLargePotion ,"GameItems.Consumables.Potions.BlueLargePotion");
			
		}
		
	}

	namespace Craftables {
		UE_DEFINE_GAMEPLAY_TAG( FireFernFruit, "GameItems.Craftables.FireFernFruit" );
		UE_DEFINE_GAMEPLAY_TAG( LuminDaisy ,"GameItems.Craftables.LuminDaisy");
		UE_DEFINE_GAMEPLAY_TAG( ScorchPetalBlossom,"GameItems.Craftables.ScorchPetalBlossom" );
		
	}
}