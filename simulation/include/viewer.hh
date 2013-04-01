#ifndef viewer_H
#define viewer_H 1
#include <Riostream.h>
#include <TGClient.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TRint.h>
#include <TROOT.h>
#include "TH1F.h"
#include <time.h>
#include "TThread.h"
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif



#ifndef ROOT_TGButton
#include "TGButton.h"
#endif

#include "TRootEmbeddedCanvas.h"

class viewer   : public TGMainFrame
{
 private:
  TRootEmbeddedCanvas*fEcanvas;
  TCanvas *fCanvas;
	TThread*vmTh;
 public:
  viewer(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~viewer();
	static void Thread(void *arg);
	virtual void draw( TH1F*);
	int Start();
	int Stop();
};
#endif
