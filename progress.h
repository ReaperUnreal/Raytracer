#ifndef PROGRESS_H
#define PROGRESS_H

class ProgressUpdater
{
public:
   ProgressUpdater();
   virtual ~ProgressUpdater();

   virtual void Update(float p) = 0;
   virtual void Init();
   virtual void Finish();
};

class CoarseMonitor : public ProgressUpdater
{
public:
   CoarseMonitor();
   virtual ~CoarseMonitor();

   virtual void Update(float p);
   virtual void Init();
   virtual void Finish();

protected:
   float prev_p;
};

#endif //PROGRESS_H
