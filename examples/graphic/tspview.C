/* ----------------------------------------------------------------------------
  tsp.C
  mbwall 17jan96
---------------------------------------------------------------------------- */
#include <cmath>
#include <cstdio>
#include <ga.h>
#include <values.h>

#include <fstream>
#include <iostream>

// comment this line if you don't have MOTIF on your system
//#define USE_MOTIF

#ifdef USE_MOTIF
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Protocols.h>
#include <Xm/PushB.h>
#include <Xm/Xm.h>
#else
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#endif

// Set this up for your favorite TSP.  The sample one is a contrived problem
// with the towns laid out in a grid (so it is easy to figure out what the
// shortest distance is, and there are many different paths with the same
// shortest path).  File format is that used by the TSPLIB problems.  You can
// grab more problems from
//
//
// Apologies for using fixed-length arrays.  But this is an example, not
// production code ;)
#define MAX_TOWNS 50
#define TSP_FILE "tsp_rect_20.txt"

float DISTANCE[MAX_TOWNS][MAX_TOWNS];
double x[MAX_TOWNS], y[MAX_TOWNS];
int ntowns = 0;
float width, height;

float Objective(GAGenome &);
int Mutator(GAGenome &, float);
void Initializer(GAGenome &);
float Comparator(const GAGenome &, const GAGenome &);
int Crossover(const GAGenome &, const GAGenome &, GAGenome *, GAGenome *);
void ERXOneChild(const GAGenome &, const GAGenome &, GAGenome *);

#ifdef USE_MOTIF

#include "bitmaps/ffst.xbm"
#include "bitmaps/ffwd.xbm"
#include "bitmaps/fwds.xbm"
#include "bitmaps/rew.xbm"
#include "bitmaps/stop.xbm"

typedef struct _BitmapInfo
{
	int width, height;
	unsigned char *bits;
} BitmapInfo;

static BitmapInfo bm[] = {
	{stop_width, stop_height, (unsigned char *)stop_bits},
	{rew_width, rew_height, (unsigned char *)rew_bits},
	{fwds_width, fwds_height, (unsigned char *)fwds_bits},
	{ffst_width, ffst_height, (unsigned char *)ffst_bits},
	{ffwd_width, ffwd_height, (unsigned char *)ffwd_bits}};

enum class
{
	bmStop,
	bmRewind,
	bmForward,
	bmForwardStop,
	bmFastForward,
	nBitmaps
};

#endif

Widget ConstructWidgets(Widget);
void QuitCB(Widget, XtPointer, XtPointer);
void InitCB(Widget, XtPointer, XtPointer);
void DrawCB(Widget, XtPointer, XtPointer);
void ResetCB(Widget, XtPointer, XtPointer);
void StopCB(Widget, XtPointer, XtPointer);
void StepCB(Widget, XtPointer, XtPointer);
void EvolveSomeCB(Widget, XtPointer, XtPointer);
void EvolveCB(Widget, XtPointer, XtPointer);
void DrawIndividual(GAGenome &, Display *, Drawable, GC, int, int);
void Refresh();

static int geninc = 10;

static char *fallbacks[] = {
	".function: 0", "*canvas.width:  500", "*canvas.height: 300",

	// motif-specific fallbacks

	"*fontList:	-*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",

	// athena-specific fallbacks

	"*font:		-*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",

	(char *)nullptr};

static XtWorkProcId procid;
static Boolean Evolve(int);
static XtAppContext appc;
static Widget canvas;
static GAGeneticAlgorithm *ga;
static GC thegc;
static int done = 0;

int main(int argc, char **argv)
{
	std::cout
		<< "Travelling salesperson demonstration program.  Use the 'ga'\n";
	std::cout
		<< "option to specify which type of genetic algorithm you would\n";
	std::cout << "like to use to do the evolution.  Options for the ga are:\n";
	std::cout << "   1 - steady-state\n";
	std::cout << "   2 - deterministic crowding\n";
	std::cout << "   3 - simple\n";
	std::cout << "\n";
	std::cout.flush();

	// read in the cities and create the DISTANCE-matrix

	double dump;
	std::ifstream in(TSP_FILE);
	if (!in)
	{
		std::cerr << "could not read data file " << TSP_FILE << "\n";
		exit(1);
	}
	ntowns = 0;
	do
	{
		in >> dump;
		if (!in.eof())
		{
			in >> x[ntowns];
			in >> y[ntowns];
			ntowns++;
		}
	} while (!in.eof() && ntowns < MAX_TOWNS);
	in.close();
	if (ntowns >= MAX_TOWNS)
	{
		std::cerr
			<< "data file contains more towns than allowed for in the fixed\n";
		std::cerr
			<< "arrays.  Recompile the program with larger arrays or try a\n";
		std::cerr << "smaller problem.\n";
		exit(1);
	}

	double dx, dy;
	int i, j;
	for (i = 0; i < ntowns; i++)
	{
		for (j = i; j < ntowns; j++)
		{
			dx = x[i] - x[j];
			dy = y[i] - y[j];
			DISTANCE[j][i] = DISTANCE[i][j] = sqrt(dx * dx + dy * dy);
		}
	}
	float minx = MAXFLOAT, maxx = MINFLOAT, miny = MAXFLOAT, maxy = MINFLOAT;
	for (i = 0; i < ntowns; i++)
	{
		minx = (minx < x[i]) ? minx : x[i];
		maxx = (maxx > x[i]) ? maxx : x[i];
	}
	for (i = 0; i < ntowns; i++)
	{
		miny = (miny < y[i]) ? miny : y[i];
		maxy = (maxy > y[i]) ? maxy : y[i];
	}
	width = maxx - minx;
	height = maxy - miny;

	// figure out which GA to use

	int whichGA = 0;

	for (int ii = 1; ii < argc; ii++)
	{
		if (strcmp("ga", argv[ii]) == 0)
		{
			if (++ii >= argc)
			{
				std::cerr << argv[0] << ": you must specify a ga:\n";
				std::cerr << "  1 - steady-state\n";
				std::cerr << "  2 - deterministic crowding\n";
				std::cerr << "  3 - simple\n";
				std::cerr << "\n";
				exit(1);
			}
			else
			{
				whichGA = atoi(argv[ii]);
				continue;
			}
		}
	}

	GAListGenome<int> genome(Objective);
	genome.initializer(::Initializer);
	genome.mutator(::Mutator);
	genome.comparator(::Comparator);
	genome.crossover(::Crossover);

	switch (whichGA)
	{
	case 2:
	{
		ga = new GADCrowdingGA(genome);
		std::cerr << "  using deterministic crowding GA...\n";
	}
	break;
	case 3:
	{
		ga = new GASimpleGA(genome);
		GASigmaTruncationScaling sigma1;
		ga->scaling(sigma1);
		std::cerr << "  using simple GA...\n";
	}
	break;
	case 1:
	default:
	{
		ga = new GASteadyStateGA(genome);
		GASigmaTruncationScaling sigma;
		ga->scaling(sigma);
		ga->set(gaNpReplacement, 0.5);
		std::cerr << "  using steady-state GA...\n";
	}
	break;
	}

	ga->minimize();
	ga->crossover(Crossover);
	ga->populationSize(50);
	ga->nGenerations(10000);
	ga->pMutation(0.3);
	ga->pCrossover(1.0);
	ga->selectScores(GAStatistics::AllScores);
	ga->parameters(argc, argv);
	ga->initialize();

	Widget toplevel =
		XtAppInitialize(&appc, "TSPView", (XrmOptionDescRec *)nullptr, 0, &argc,
						argv, fallbacks, (ArgList)nullptr, 0);
	Widget shell = ConstructWidgets(toplevel);

	XtRealizeWidget(shell);
	XtMapWidget(shell);

	static Atom wmDeleteWindow;
	wmDeleteWindow =
		XInternAtom(XtDisplay(toplevel), "WM_DELETE_WINDOW", False);
#ifdef USE_MOTIF
	XmAddWMProtocolCallback(shell, wmDeleteWindow, QuitCB, (XtPointer)0);
#else
	XSetWMProtocols(XtDisplay(toplevel), XtWindow(toplevel), &wmDeleteWindow,
					1);
#endif

	XtGCMask valueMask = GCFunction | GCLineWidth;
	XGCValues gcValues;
	gcValues.function = GXcopy;
	gcValues.line_width = 2;
	thegc =
		XCreateGC(XtDisplay(toplevel), RootWindowOfScreen(XtScreen(toplevel)),
				  valueMask, &gcValues);

	while (!done)
	{
		XEvent event;
		XtAppNextEvent(appc, &event);
		XtDispatchEvent(&event);
	}

	XFreeGC(XtDisplay(toplevel), thegc);
	delete ga;

	return 0;
}

Boolean Evolve(int n)
{
	if ((n < 0 && ga->done() == false) || ga->generation() < n)
	{
		ga->step();
		if (ga->generation() % 10 == 0)
		{
			std::cerr << "generation: " << ga->generation() << "\t";
			std::cerr << "best score is " << ga->population().best().score()
					  << "\n";
		}
		Refresh();
		return False;
	}

	return True;
}

void ResetCB(Widget, XtPointer cd, XtPointer)
{
	auto *ga = (GAGeneticAlgorithm *)cd;
	if (procid)
	{
		XtRemoveWorkProc(procid);
		procid = 0;
	}
	std::cerr << "initialized\n";
	ga->initialize();
	Refresh();
}

void StopCB(Widget, XtPointer, XtPointer)
{
	if (procid)
	{
		XtRemoveWorkProc(procid);
		procid = 0;
	}
}

void StepCB(Widget, XtPointer cd, XtPointer)
{
	auto *ga = (GAGeneticAlgorithm *)cd;
	Evolve(ga->generation() + 1);
}

void EvolveSomeCB(Widget, XtPointer cd, XtPointer)
{
	auto *ga = (GAGeneticAlgorithm *)cd;
	procid = XtAppAddWorkProc(appc, (XtWorkProc)Evolve,
							  (XtPointer)(ga->generation() + geninc));
}

void EvolveCB(Widget, XtPointer, XtPointer)
{
	procid = XtAppAddWorkProc(appc, (XtWorkProc)Evolve, (XtPointer)(-1));
}

void QuitCB(Widget, XtPointer, XtPointer) { done = 1; }

#define SCALE 10
#define BUF 10

void DrawCB(Widget w, XtPointer cd, XtPointer)
{
	auto *ga = (GAGeneticAlgorithm *)cd;
	XClearWindow(XtDisplay(w), XtWindow(w));
	int nrows = (int)sqrt(1.3333333333 * ga->population().size()) + 1;
	int ncols = ga->population().size() / nrows + 1;
	int idx = 0;
	for (int i = 0; i < nrows && idx < ga->population().size(); i++)
	{
		for (int j = 0; j < ncols && idx < ga->population().size(); j++)
		{
			int a = BUF + i * (SCALE * width + BUF);
			int b = BUF + j * (SCALE * height + BUF);
			DrawIndividual(ga->population().best(idx++), XtDisplay(w),
						   XtWindow(w), thegc, a, b);
		}
	}
}

void Refresh() { DrawCB(canvas, (XtPointer)ga, nullptr); }

// draw an individual genome at the specified coordinates (in Xwindows space)
// Draw the paths that link the towns.  We draw a complete, closed loop
// so go for one more than the number of nodes in the list.
void DrawIndividual(GAGenome &g, Display *display, Drawable drawable, GC gc,
					int xx, int yy)
{
	auto &genome = (GAListGenome<int> &)g;
	for (int i = 0; i < genome.size() + 1; i++)
	{
		int sidx = *genome.current();
		int eidx = *genome.next();
		int a = xx + SCALE * x[sidx];
		int b = yy + SCALE * y[sidx];
		int c = xx + SCALE * x[eidx];
		int d = yy + SCALE * y[eidx];
		XDrawLine(display, drawable, gc, a, b, c, d);
	}
}

#undef SCALE
#undef BUF

// Here are the genome operators that we want to use for this problem.
float Objective(GAGenome &g)
{
	auto &genome = (GAListGenome<int> &)g;
	float dist = 0;
	if (genome.head())
	{
		for (int i = 0; i < ntowns; i++)
			dist += DISTANCE[*genome.current()][*genome.next()];
	}
	return dist;
}

void Initializer(GAGenome &g)
{
	auto &child = (GAListGenome<int> &)g;
	while (child.head())
		child.destroy(); // destroy any pre-existing list

	int i, town;
	static int visit[MAX_TOWNS];

	memset(visit, 0, MAX_TOWNS * sizeof(int));
	town = GARandomInt(0, ntowns - 1);
	visit[town] = 1;
	child.insert(town, GAListBASE::HEAD); // the head node

	for (i = 1; i < ntowns; i++)
	{
		do
		{
			town = GARandomInt(0, ntowns - 1);
		} while (visit[town]);
		visit[town] = 1;
		child.insert(town);
	} // each subsequent node
}

int Mutator(GAGenome &g, float pmut)
{
	auto &child = (GAListGenome<int> &)g;
	int n, i;
	if ((GARandomFloat() >= pmut) || (pmut <= 0))
		return 0;

	n = child.size();

	if (GARandomFloat() < 0.5)
	{
		child.swap(GARandomInt(0, n - 1),
				   GARandomInt(0, n - 1)); // swap only one time
	}
	else
	{
		int nNodes = GARandomInt(1, ((int)(n / 2 - 1))); // displace nNodes
		child.warp(GARandomInt(0, n - 1)); // with or without
		GAList<int> TmpList; // inversion
		for (i = 0; i < nNodes; i++)
		{
			int *iptr = child.remove();
			TmpList.insert(*iptr, GAListBASE::AFTER);
			delete iptr;
			child.next();
		}
		int invert;
		child.warp(GARandomInt(0, n - nNodes));
		invert = (GARandomFloat() < 0.5) ? 0 : 1;
		if (invert)
			TmpList.head();
		else
			TmpList.tail();

		for (i = 0; i < nNodes; i++)
		{
			int *iptr = TmpList.remove();
			child.insert(*iptr, GAListBASE::AFTER);
			delete iptr;
			if (invert)
				TmpList.prev();
			else
				TmpList.next();
		}
	}
	child.head(); // set iterator to root node

	return (1);
}

int Crossover(const GAGenome &g1, const GAGenome &g2, GAGenome *c1,
			  GAGenome *c2)
{
	int nc = 0;
	if (c1)
	{
		ERXOneChild(g1, g2, c1);
		nc += 1;
	}
	if (c2)
	{
		ERXOneChild(g1, g2, c2);
		nc += 1;
	}
	return nc;
}

void ERXOneChild(const GAGenome &g1, const GAGenome &g2, GAGenome *c1)
{
	auto &mate1 = (GAListGenome<int> &)g1;
	auto &mate2 = (GAListGenome<int> &)g2;
	auto &sis = (GAListGenome<int> &)*c1;

	int i, j, k, t1, t2, town;

	static char CM[MAX_TOWNS][MAX_TOWNS], visit[MAX_TOWNS];
	memset(CM, 0, MAX_TOWNS * MAX_TOWNS * sizeof(char));
	memset(visit, 0, MAX_TOWNS * sizeof(char));

	while (sis.head())
		sis.destroy();

	// create connection matrix
	mate1.head();
	for (j = 0; j < ntowns; j++)
	{
		t1 = *mate1.current();
		t2 = *mate1.next();
		CM[t1][t2] = 1;
		CM[t2][t1] = 1;
	}
	mate2.head();
	for (j = 0; j < ntowns; j++)
	{
		t1 = *mate2.current();
		t2 = *mate2.next();
		CM[t1][t2] = 1;
		CM[t2][t1] = 1;
	}

	// select 1st town randomly
	town = GARandomInt(0, ntowns - 1);
	visit[town] = 1;
	memset(CM[town], 0, MAX_TOWNS * sizeof(char));
	sis.insert(town); // the head node

	GAList<int> PossFollowList;
	GAList<int> FollowersList[5];
	while (PossFollowList.head())
		PossFollowList.destroy();
	for (k = 0; k < 5; k++)
	{
		while (FollowersList[k].head())
			FollowersList[k].destroy();
	}

	// select the following town with the minimal no of next folling towns
	int nPoss, nFollow;
	for (i = 1; i < ntowns; i++)
	{
		nPoss = 0;
		for (j = 0; j < ntowns; j++)
		{ // no of poss. following towns
			if (CM[j][town])
			{
				nPoss += 1;
				PossFollowList.insert(j);
			}
		}
		// nPoss = 0;
		if (nPoss == 0)
		{
			do
			{
				town = GARandomInt(0, ntowns - 1);
			} while (visit[town]); // no follower
			visit[town] = 1;
			memset(CM[town], 0, MAX_TOWNS * sizeof(char));
			sis.insert(town);
		}
		else
		{
			PossFollowList.head();
			for (j = 0; j < nPoss; j++)
			{
				nFollow = 0;
				town = (*PossFollowList.current());
				for (k = 0; k < ntowns; k++)
				{
					if (CM[k][town])
						nFollow++;
				}
				FollowersList[nFollow].insert(town);
				PossFollowList.next();
			}
			k = 0;
			while (FollowersList[k].size() == 0)
				k++;
			FollowersList[k].warp(GARandomInt(0, FollowersList[k].size()));
			town = (*FollowersList[k].current());
			visit[town] = 1;
			memset(CM[town], 0, MAX_TOWNS * sizeof(char));
			sis.insert(town);
		}
		while (PossFollowList.head())
			PossFollowList.destroy();
		for (k = 0; k < 5; k++)
		{
			while (FollowersList[k].head())
				FollowersList[k].destroy();
		}
	}
	sis.head(); // set iterator to head of list
}

float Comparator(const GAGenome &g1, const GAGenome &g2)
{
	auto &a = (GAListGenome<int> &)g1;
	auto &b = (GAListGenome<int> &)g2;

	int i, j, t1, t2;
	float dist = ntowns;

	static char CM1[MAX_TOWNS][MAX_TOWNS], CM2[MAX_TOWNS][MAX_TOWNS];
	memset(CM1, 0, MAX_TOWNS * MAX_TOWNS * sizeof(char));
	memset(CM2, 0, MAX_TOWNS * MAX_TOWNS * sizeof(char));

	// create connection matrix CM1
	a.head();
	for (i = 0; i < ntowns; i++)
	{
		t1 = *a.current();
		t2 = *a.next();
		CM1[t1][t2] = 1;
		CM1[t2][t1] = 1;
	}
	// create connection matrix CM2
	b.head();
	for (i = 0; i < ntowns; i++)
	{
		t1 = *b.current();
		t2 = *b.next();
		CM2[t1][t2] = 1;
		CM2[t2][t1] = 1;
	}
	// calc distance = how many edges are different
	for (i = 0; i < ntowns; i++)
	{
		for (j = i; j < ntowns; j++)
		{
			if (CM1[i][j] & CM2[i][j])
				dist--;
		}
	}
	return (dist);
}

//   Here we override the _write method for the List class.  This lets us see
// exactly what we want (the default _write method dumps out pointers to the
// data rather than the data contents).
//   This routine prints out the contents of each element of the list,
// separated by a space.  It does not put a newline at the end of the list.
//   Notice that you can override ANY function of a template class.  This is
// called "specialization" in C++ and it lets you tailor the behaviour of a
// template class to better fit the type.
template <> int GAListGenome<int>::write(std::ostream &os) const
{
	int *cur, *head;
	GAListIter<int> iter(*this);
	if ((head = iter.head()) != nullptr)
		os << *head << " ";
	for (cur = iter.next(); cur && cur != head; cur = iter.next())
		os << *cur << " ";

	return os.fail() ? 1 : 0;
}

// If your compiler does not do automatic instantiation (e.g. g++ 2.6.8),
// then define the NO_AUTO_INST directive.
#ifdef NO_AUTO_INST
#include <GAList.C>
#include <GAListGenome.C>
template class GAList<int>;
template class GAListGenome<int>;
#endif

// Here are two versions of the graphic interface.  One version for those of
// you with MOTIF on your systems, and one version for those of you with only
// the athena widget set.  Sorry, no Windoze version yet...

#ifdef USE_MOTIF

Widget ConstructWidgets(Widget toplevel)
{
	Widget shell =
		XtVaCreatePopupShell("shell", topLevelShellWidgetClass, toplevel, NULL);

	Widget form =
		XtVaCreateManagedWidget("form", xmFormWidgetClass, shell, NULL);

	Pixmap pix;
	Pixel fg, bg;
	unsigned int depth;
	XtVaGetValues(form, XmNforeground, &fg, XmNbackground, &bg, XmNdepth,
				  &depth, NULL);
	pix = XCreatePixmapFromBitmapData(
		XtDisplay(form), RootWindowOfScreen(XtScreen(form)),
		(char *)bm[bmRewind].bits, bm[bmRewind].width, bm[bmRewind].height, fg,
		bg, depth);
	Widget rewind = XtVaCreateManagedWidget(
		"rewind", xmPushButtonWidgetClass, form, XmNbottomAttachment,
		XmATTACH_FORM, XmNlabelType, XmPIXMAP, XmNlabelPixmap, pix, NULL);
	pix = XCreatePixmapFromBitmapData(XtDisplay(form),
									  RootWindowOfScreen(XtScreen(form)),
									  (char *)bm[bmStop].bits, bm[bmStop].width,
									  bm[bmStop].height, fg, bg, depth);
	Widget stop = XtVaCreateManagedWidget(
		"stop", xmPushButtonWidgetClass, form, XmNleftAttachment,
		XmATTACH_WIDGET, XmNleftWidget, rewind, XmNbottomAttachment,
		XmATTACH_FORM, XmNlabelType, XmPIXMAP, XmNlabelPixmap, pix, NULL);
	pix = XCreatePixmapFromBitmapData(
		XtDisplay(form), RootWindowOfScreen(XtScreen(form)),
		(char *)bm[bmForward].bits, bm[bmForward].width, bm[bmForward].height,
		fg, bg, depth);
	Widget step = XtVaCreateManagedWidget(
		"step", xmPushButtonWidgetClass, form, XmNleftAttachment,
		XmATTACH_WIDGET, XmNleftWidget, stop, XmNbottomAttachment,
		XmATTACH_FORM, XmNlabelType, XmPIXMAP, XmNlabelPixmap, pix, NULL);
	pix = XCreatePixmapFromBitmapData(
		XtDisplay(form), RootWindowOfScreen(XtScreen(form)),
		(char *)bm[bmForwardStop].bits, bm[bmForwardStop].width,
		bm[bmForwardStop].height, fg, bg, depth);
	Widget some = XtVaCreateManagedWidget(
		"some", xmPushButtonWidgetClass, form, XmNleftAttachment,
		XmATTACH_WIDGET, XmNleftWidget, step, XmNbottomAttachment,
		XmATTACH_FORM, XmNlabelType, XmPIXMAP, XmNlabelPixmap, pix, NULL);
	pix = XCreatePixmapFromBitmapData(
		XtDisplay(form), RootWindowOfScreen(XtScreen(form)),
		(char *)bm[bmFastForward].bits, bm[bmFastForward].width,
		bm[bmFastForward].height, fg, bg, depth);
	Widget evolve = XtVaCreateManagedWidget(
		"evolve", xmPushButtonWidgetClass, form, XmNleftAttachment,
		XmATTACH_WIDGET, XmNleftWidget, some, XmNbottomAttachment,
		XmATTACH_FORM, XmNlabelType, XmPIXMAP, XmNlabelPixmap, pix, NULL);
	XtAddCallback(rewind, XmNactivateCallback, ResetCB, (XtPointer)ga);
	XtAddCallback(stop, XmNactivateCallback, StopCB, (XtPointer)ga);
	XtAddCallback(step, XmNactivateCallback, StepCB, (XtPointer)ga);
	XtAddCallback(some, XmNactivateCallback, EvolveSomeCB, (XtPointer)ga);
	XtAddCallback(evolve, XmNactivateCallback, EvolveCB, (XtPointer)ga);

	canvas = XtVaCreateManagedWidget(
		"canvas", xmDrawingAreaWidgetClass, form, XmNtopAttachment,
		XmATTACH_FORM, XmNrightAttachment, XmATTACH_FORM, XmNleftAttachment,
		XmATTACH_FORM, XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget,
		rewind, NULL);
	XtAddCallback(canvas, XmNexposeCallback, DrawCB, (XtPointer)ga);

	return shell;
}

#else

void ExposureEH(Widget w, XtPointer cd, XEvent *, Boolean *)
{
	DrawCB(w, cd, nullptr);
}

Widget ConstructWidgets(Widget toplevel)
{
	Widget form =
		XtVaCreateManagedWidget("form", formWidgetClass, toplevel, NULL);

	canvas = XtVaCreateManagedWidget("canvas", widgetClass, form, NULL);
	XtAddEventHandler(canvas, ExposureMask, False, ExposureEH, (XtPointer)ga);

	Widget ctrlbox =
		XtVaCreateManagedWidget("controls", boxWidgetClass, form, XtNfromVert,
								canvas, XtNorientation, "vertical", NULL);
	Widget rewind =
		XtVaCreateManagedWidget("rewind", commandWidgetClass, ctrlbox, NULL);
	Widget stop =
		XtVaCreateManagedWidget("stop", commandWidgetClass, ctrlbox, NULL);
	Widget step =
		XtVaCreateManagedWidget("step", commandWidgetClass, ctrlbox, NULL);
	Widget some =
		XtVaCreateManagedWidget("some", commandWidgetClass, ctrlbox, NULL);
	Widget evolve =
		XtVaCreateManagedWidget("evolve", commandWidgetClass, ctrlbox, NULL);

	XtAddCallback(rewind, XtNcallback, ResetCB, (XtPointer)ga);
	XtAddCallback(stop, XtNcallback, StopCB, (XtPointer)ga);
	XtAddCallback(step, XtNcallback, StepCB, (XtPointer)ga);
	XtAddCallback(some, XtNcallback, EvolveSomeCB, (XtPointer)ga);
	XtAddCallback(evolve, XtNcallback, EvolveCB, (XtPointer)ga);

	Widget quit =
		XtVaCreateManagedWidget("quit", commandWidgetClass, ctrlbox, NULL);
	XtAddCallback(quit, XtNcallback, QuitCB, (XtPointer)nullptr);

	return toplevel;
}

#endif
