/// \file
/// \ingroup tutorial_roofit
/// \notebook -nodraw
///
/// Data and categories: working with RooCategory objects to describe discrete variables
///
/// \macro_output
/// \macro_code
///
/// \date 07/2008
/// \author Wouter Verkerke

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPolynomial.h"
#include "RooCategory.h"
#include "Roo1DTable.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
using namespace RooFit;

void rf404_categories()
{

   // C o n s t r u c t    a   c a t e g o r y   w i t h   l a b e l s
   // ----------------------------------------------------------------

   // Define a category with labels only
   RooCategory tagCat("tagCat", "Tagging category");
   tagCat.defineType("Lepton");
   tagCat.defineType("Kaon");
   tagCat.defineType("NetTagger-1");
   tagCat.defineType("NetTagger-2");
   tagCat.Print();

   // C o n s t r u c t    a   c a t e g o r y   w i t h   l a b e l s   a n d   i n d e c e s
   // ----------------------------------------------------------------------------------------

   // Define a category with explicitly numbered states
   RooCategory b0flav("b0flav", "B0 flavour eigenstate");
   b0flav.defineType("B0", -1);
   b0flav.defineType("B0bar", 1);
   b0flav.Print();

   // G e n e r a t e   d u m m y   d a t a  f o r   t a b u l a t i o n   d e m o
   // ----------------------------------------------------------------------------

   // Generate a dummy dataset
   RooRealVar x("x", "x", 0, 10);
   RooDataSet *data = RooPolynomial("p", "p", x).generate(RooArgSet(x, b0flav, tagCat), 10000);

   // P r i n t   t a b l e s   o f   c a t e g o r y   c o n t e n t s   o f   d a t a s e t s
   // ------------------------------------------------------------------------------------------

   // Tables are equivalent of plots for categories
   Roo1DTable *btable = data->table(b0flav);
   btable->Print();
   btable->Print("v");

   // Create table for subset of events matching cut expression
   Roo1DTable *ttable = data->table(tagCat, "x>8.23");
   ttable->Print();
   ttable->Print("v");

   // Create table for all (tagCat x b0flav) state combinations
   Roo1DTable *bttable = data->table(RooArgSet(tagCat, b0flav));
   bttable->Print("v");

   // Retrieve number of events from table
   // Number can be non-integer if source dataset has weighed events
   Double_t nb0 = btable->get("B0");
   cout << "Number of events with B0 flavor is " << nb0 << endl;

   // Retrieve fraction of events with "Lepton" tag
   Double_t fracLep = ttable->getFrac("Lepton");
   cout << "Fraction of events tagged with Lepton tag is " << fracLep << endl;

   // D e f i n i n g   r a n g e s   f o r   p l o t t i n g ,   f i t t i n g   o n   c a t e g o r i e s
   // ------------------------------------------------------------------------------------------------------

   // Define named range as comma separated list of labels
   tagCat.setRange("good", "Lepton,Kaon");

   // Or add state names one by one
   tagCat.addToRange("soso", "NetTagger-1");
   tagCat.addToRange("soso", "NetTagger-2");

   // Use category range in dataset reduction specification
   RooDataSet *goodData = (RooDataSet *)data->reduce(CutRange("good"));
   goodData->table(tagCat)->Print("v");
}
