#include "SCCmnPrivate.h"
#include "MasterDB.h"

#	define CHECK_MACRO_FUNCTIONS
#	define IsPositive(x) (x > 0)						// �������?
#	define IsPositiveIncludeZero(x) (x >= 0)			// 0�� �����ϴ� �������?
#	define IsNotZero(x) (x != 0)						// 0�� �ƴ���?

bool MDBHeroStat::IsValid() const
{
    if (false == IsPositive(Seq)
        || false == IsPositiveIncludeZero(Stat.Info.STR)
        || false == IsPositiveIncludeZero(Stat.Info.DEX)
        || false == IsPositiveIncludeZero(Stat.Info.LUK)
        || false == IsPositiveIncludeZero(Stat.Info.WIS))
        return false;
    return true;
}
