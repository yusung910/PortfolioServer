#include "SCCmnPrivate.h"
#include "MasterDB.h"

#	define CHECK_MACRO_FUNCTIONS
#	define IsPositive(x) (x > 0)						// 양수인지?
#	define IsPositiveIncludeZero(x) (x >= 0)			// 0을 포함하는 양수인지?
#	define IsNotZero(x) (x != 0)						// 0이 아닌지?

bool MDBObjectStatistics::IsValid() const
{
    if (false == IsPositive(ObjectStatID)
        || false == IsPositiveIncludeZero(Stat.Info.Strength)
        || false == IsPositiveIncludeZero(Stat.Info.Dexterity)
        || false == IsPositiveIncludeZero(Stat.Info.Endurance)
        || false == IsPositiveIncludeZero(Stat.Info.WeaponMastery)

        || false == IsPositiveIncludeZero(Stat.Info.HP)
        || false == IsPositiveIncludeZero(Stat.Info.MaxHP)
        || false == IsPositiveIncludeZero(Stat.Info.MP)
        || false == IsPositiveIncludeZero(Stat.Info.MaxMP)

        || false == IsPositiveIncludeZero(Stat.Info.MeleeMaxDamage)
        || false == IsPositiveIncludeZero(Stat.Info.MeleeMinDamage)
        || false == IsPositiveIncludeZero(Stat.Info.MeleeDefence)
        )
        return false;
    return true;
}