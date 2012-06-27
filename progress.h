#ifndef PROGRESS_H
#define PROGRESS_H

class ProgressUpdater
{
public:
   ProgressUpdater();
   virtual ~ProgressUpdater();

   virtual void Update(float p) = 0;
};

#endif //PROGRESS_H
