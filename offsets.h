"UWorlds": "0x12DA3A98",
"OwningWorld": "0xC0",
"GameInstance": "0x208",
"LocalPlayers": "0x38",
"PlayerController": "0x30",
"LocalPawn": "0x350",
"PlayerState": "0x2B0",
"RootComponent": "0x1B0",
"GameState": "0x190",
"PersistentLevel": "0x40",
"ComponetToWorld": "0x1c0",
"CameraManager": "0x348",
"Mesh": "0x328",
"PawnPrivate": "0x320",
"PlayerArray": "0x2C0",
"RootComponet": "0x198",
"pawn_private": "0x320",
"AActor": "0xA0",
"ActorCount": "0xA8",
"BoneArray": "0x580",
"pNameStructure": "0xAD0"

camera:

struct FPlane : Vector3
{
	double W;

	FPlane() : W(0) { }
	FPlane(double W) : W(W) { }
};

class FMatrix
{
public:
	double m[4][4];
	FPlane XPlane, YPlane, ZPlane, WPlane;

	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() { }
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane)
		: XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) { }
};

inline double RadiansToDegrees(double dRadians)
{
	return dRadians * (180.0 / M_PI);
}

template< typename t >
class TArray
{
public:

	TArray() : tData(), iCount(), iMaxCount() { }
	TArray(t* data, int count, int max_count) :
		tData(tData), iCount(iCount), iMaxCount(iMaxCount) { }

public:

	auto Get(int idx) -> t
	{
		return read< t >(reinterpret_cast<__int64>(this->tData) + (idx * sizeof(t)));
	}

	auto Size() -> std::uint32_t
	{
		return this->iCount;
	}

	bool IsValid()
	{
		return this->iCount != 0;
	}

	t* tData;
	int iCount;
	int iMaxCount;
};

auto GetViewState() -> uintptr_t
{
	TArray<uintptr_t> ViewState = read<TArray<uintptr_t>>(cache::local_players + 0xD0);
	return ViewState.Get(1);
}

Camera get_view_point()
{
	auto mProjection = read<FMatrix>(GetViewState() + 0x900);
	view_point.rotation.x = RadiansToDegrees(std::asin(mProjection.ZPlane.W));
	view_point.rotation.y = RadiansToDegrees(std::atan2(mProjection.YPlane.W, mProjection.XPlane.W));
	view_point.rotation.z = 0.0;

	view_point.location.x = mProjection.m[3][0];
	view_point.location.y = mProjection.m[3][1];
	view_point.location.z = mProjection.m[3][2];
	float FieldOfView = atanf(1 / read<double>(GetViewState() + 0x700)) * 2;
	view_point.fov = (FieldOfView) * (180.f / M_PI); return view_point;
}

credits to @Shadows4s

32.00 updated
