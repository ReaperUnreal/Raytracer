#include "main.h"


// functions for the abstract interface
ProgressUpdater::ProgressUpdater()
{
}

ProgressUpdater::~ProgressUpdater()
{
}

void ProgressUpdater::Init()
{
}

void ProgressUpdater::Finish()
{
}


// for the coarse updater
CoarseMonitor::CoarseMonitor() : prev_p(0.0f)
{
}

CoarseMonitor::~CoarseMonitor()
{
}

void CoarseMonitor::Update(float p)
{
   if(fabsf(p - prev_p) >= 0.1f)
   {
      printf("\b\b\b\b%3d%%", static_cast<int>(fmin(fmax(p, 0.0f), 1.0f) * 100.0f));
      fflush(stdout);
      prev_p = p;
   }
}

void CoarseMonitor::Init()
{
   printf("%3d%%", 0);
   fflush(stdout);
   prev_p = 0.0f;
}

void CoarseMonitor::Finish()
{
   printf("\b\b\b\b100%%\n");
}

// for the percentage updater
PercentMonitor::PercentMonitor() : prev_p(0.0f)
{
}

PercentMonitor::~PercentMonitor()
{
}

void PercentMonitor::Update(float p)
{
   if(fabsf(p - prev_p) >= 0.01f)
   {
      printf("\b\b\b\b%3d%%", static_cast<int>(fmin(fmax(p, 0.0f), 1.0f) * 100.0f));
      fflush(stdout);
      prev_p = p;
   }
}

void PercentMonitor::Init()
{
   printf("%3d%%", 0);
   fflush(stdout);
   prev_p = 0.0f;
}

void PercentMonitor::Finish()
{
   printf("\b\b\b\b100%%\n");
}
