#pragma once

#define DRAWSPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Blue, true);
#define DRAWLINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true, -1.f, 0, 1.f);
#define DRAWPOINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Blue, true);
#define DRAWVECTOR(StartLocation, EndLocation) if (GetWorld()) \
{ \
DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true, -1.f, 0, 1.f); \
DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Blue, true); \
}