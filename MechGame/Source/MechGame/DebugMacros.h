#pragma once

#define DRAWSPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Blue, true);
#define DRAWSPHERE1FRAME(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Blue, false, -1.f);
#define DRAWSPHEREWITHCOLOUR(Location, Colour)  if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 24, Colour, false, 5.f);
#define DRAWLINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true, -1.f, 0, 1.f);
#define DRAWLINE1FRAME(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.f, 0, 1.f);
#define DRAWPOINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Blue, true);
#define DRAWPOINT1FRAME(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Blue, false, -1.f);
#define DRAWCUBE(Location, Size) if(GetWorld()) DrawDebugBox(GetWorld(), Location, Size, FColor::Blue, true);
#define DRAWVECTOR(StartLocation, EndLocation) if (GetWorld()) \
{ \
DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true, -1.f, 0, 1.f); \
DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Blue, true); \
}
#define DRAWVECTOR1FRAME(StartLocation, EndLocation) if (GetWorld()) \
{ \
DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.f, 0, 1.f); \
DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Blue, false, -1.f); \
}