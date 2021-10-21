// 2009 © Václav Šmilauer <eudoxos@arcig.cz>
#include<sudodem/lib/base/Math.hpp>
#include<sudodem/pkg/dem/DiskPack.hpp>
#include<sudodem/lib/pyutil/doc_opts.hpp>
//#include<sudodem/lib/base/Math.hpp>

BOOST_PYTHON_MODULE(_packDisks){
  boost::python::scope().attr("__doc__")="Creation, manipulation, IO for generic disk packings.";
	SUDODEM_SET_DOCSTRING_OPTS;
  boost::python::class_<DiskPack>("DiskPack","Set of disks represented as centers and radii. This class is returned by :yref:`sudodem.pack.randomDensePack`, :yref:`sudodem.pack.randomPeriPack` and others. The object supports iteration over disks, as in \n\n\t>>> sp=DiskPack()\n\t>>> for center,radius in sp: print center,radius\n\n\t>>> for disk in sp: print disk[0],disk[1]   ## same, but without unpacking the tuple automatically\n\n\t>>> for i in range(0,len(sp)): print sp[i][0], sp[i][1]   ## same, but accessing disks by index\n\n\n.. admonition:: Special constructors\n\n\tConstruct from list of ``[(c1,r1),(c2,r2),…]``. To convert two same-length lists of ``centers`` and ``radii``, construct with ``zip(centers,radii)``.\n",boost::python::init<boost::python::optional<boost::python::list> >(boost::python::args("list"),"Empty constructor, optionally taking list [ ((cx,cy,cz),r), … ] for initial data." ))
		.def("add",&DiskPack::add,"Add single disk to packing, given center as 3-tuple and radius")
		.def("toList",&DiskPack::toList,"Return packing data as python list.")
		.def("fromList",&DiskPack::fromList,"Make packing from given list, same format as for constructor. Discards current data.")
		.def("fromList",&DiskPack::fromLists,(boost::python::arg("centers"),boost::python::arg("radii")),"Make packing from given list, same format as for constructor. Discards current data.")
		.def("load",&DiskPack::fromFile,(boost::python::arg("fileName")),"Load packing from external text file (current data will be discarded).")
		.def("save",&DiskPack::toFile,(boost::python::arg("fileName")),"Save packing to external text file (will be overwritten).")
		.def("fromSimulation",&DiskPack::fromSimulation,"Make packing corresponding to the current simulation. Discards current data.")
		.def("makeCloud",&DiskPack::makeCloud,(boost::python::arg("minCorner")=Vector3r(Vector3r::Zero()),boost::python::arg("maxCorner")=Vector3r(Vector3r::Zero()),boost::python::arg("rMean")=-1,boost::python::arg("rRelFuzz")=0,boost::python::arg("num")=-1,boost::python::arg("periodic")=false,boost::python::arg("porosity")=0.65,boost::python::arg("psdSizes")=vector<Real>(),boost::python::arg("psdCumm")=vector<Real>(),boost::python::arg("distributeMass")=false,boost::python::arg("seed")=0,boost::python::arg("hSize")=Matrix3r(Matrix3r::Zero())),"Create random loose packing enclosed in a parallelepiped (also works in 2D if minCorner[k]=maxCorner[k] for one coordinate)."
		"\nDisk radius distribution can be specified using one of the following ways:\n\n#. *rMean*, *rRelFuzz* and *num* gives uniform radius distribution in *rMean×(1 ± rRelFuzz)*. Less than *num* disks can be generated if it is too high.\n#. *rRelFuzz*, *num* and (optional) *porosity*, which estimates mean radius so that *porosity* is attained at the end.  *rMean* must be less than 0 (default). *porosity* is only an initial guess for the generation algorithm, which will retry with higher porosity until the prescibed *num* is obtained.\n#. *psdSizes* and *psdCumm*, two arrays specifying points of the `particle size distribution <http://en.wikipedia.org/wiki/Particle_size_distribution>`__ function. As many disks as possible are generated.\n#. *psdSizes*, *psdCumm*, *num*, and (optional) *porosity*, like above but if *num* is not obtained, *psdSizes* will be scaled down uniformly, until *num* is obtained (see :yref:`appliedPsdScaling<sudodem._packDisks.DiskPack.appliedPsdScaling>`).\n\nBy default (with ``distributeMass==False``), the distribution is applied to particle radii. The usual sense of \"particle size distribution\" is the distribution of *mass fraction* (rather than particle count); this can be achieved with ``distributeMass=True``."
		"\n\nIf *num* is defined, then sizes generation is deterministic, giving the best fit of target distribution. It enables disks placement in descending size order, thus giving lower porosity than the random generation."
		"\n\n:param Vector3 minCorner: lower corner of an axis-aligned box\n:param Vector3 maxCorner: upper corner of an axis-aligned box\n:param Matrix3 hSize: base vectors of a generalized box (arbitrary parallelepiped, typically :yref:`Cell::hSize`), superseeds minCorner and maxCorner if defined. For periodic boundaries only.\n:param float rMean: mean radius or disks\n:param float rRelFuzz: dispersion of radius relative to rMean\n:param int num: number of disks to be generated. If negavite (default), generate as many as possible with stochastic sizes, ending after a fixed number of tries to place the disk in space, else generate exactly *num* disks with deterministic size distribution.\n:param bool periodic: whether the packing to be generated should be periodic\n:param float porosity: initial guess for the iterative generation procedure (if *num*>1). The algorithm will be retrying until the number of generated disks is *num*. The first iteration tries with the provided porosity, but next iterations increase it if necessary (hence an initialy high porosity can speed-up the algorithm). If *psdSizes* is not defined, *rRelFuzz* ($z$) and *num* ($N$) are used so that the porosity given ($\\rho$) is approximately achieved at the end of generation, $r_m=\\sqrt[3]{\\frac{V(1-\\rho)}{\\frac{4}{3}\\pi(1+z^2)N}}$. The default is $\\rho$=0.5. The optimal value depends on *rRelFuzz* or  *psdSizes*.\n:param psdSizes: sieve sizes (particle diameters) when particle size distribution (PSD) is specified\n:param psdCumm: cummulative fractions of particle sizes given by *psdSizes*; must be the same length as *psdSizes* and should be non-decreasing\n:param bool distributeMass: if ``True``, given distribution will be used to distribute disk's mass rather than radius of them.\n:param seed: number used to initialize the random number generator.\n:returns: number of created disks, which can be lower than *num* depending on the method used.\n")
		.def("psd",&DiskPack::psd,(boost::python::arg("bins")=50,boost::python::arg("mass")=true),"Return `particle size distribution <http://en.wikipedia.org/wiki/Particle_size_distribution>`__ of the packing.\n:param int bins: number of bins between minimum and maximum diameter\n:param mass: Compute relative mass rather than relative particle count for each bin. Corresponds to :yref:`distributeMass parameter for makeCloud<sudodem.pack.DiskPack.makeCloud>`.\n:returns: tuple of ``(cumm,edges)``, where ``cumm`` are cummulative fractions for respective diameters  and ``edges`` are those diameter values. Dimension of both arrays is equal to ``bins+1``.")
		// new psd
		.def("particleSD",&DiskPack::particleSD,(boost::python::arg("minCorner"),boost::python::arg("maxCorner"),boost::python::arg("rMean"),boost::python::arg("periodic")=false,boost::python::arg("name"),boost::python::arg("numSph"),boost::python::arg("radii")=vector<Real>(),boost::python::arg("passing")=vector<Real>(),boost::python::arg("passingIsNotPercentageButCount")=false, boost::python::arg("seed")=0),"Create random packing enclosed in box given by minCorner and maxCorner, containing numSph disks. Returns number of created disks, which can be < num if the packing is too tight. The computation is done according to the given psd.")
		.def("particleSD2",&DiskPack::particleSD2,(boost::python::arg("radii"),boost::python::arg("passing"),boost::python::arg("numSph"),boost::python::arg("periodic")=false,boost::python::arg("cloudPorosity")=.8,boost::python::arg("seed")=0),"Create random packing following the given particle size distribution (radii and volume/mass passing for each fraction) and total number of particles *numSph*. The cloud size (periodic or aperiodic) is computed from the PSD and is always cubic.")
		.def("particleSD_2d",&DiskPack::particleSD_2d,(boost::python::arg("minCorner"),boost::python::arg("maxCorner"),boost::python::arg("rMean"),boost::python::arg("periodic")=false,boost::python::arg("name"),boost::python::arg("numSph"),boost::python::arg("radii")=vector<Real>(),boost::python::arg("passing")=vector<Real>(),boost::python::arg("passingIsNotPercentageButCount")=false,boost::python::arg("seed")=0),"Create random packing on XY plane, defined by minCorner and maxCorner, containing numSph disks. Returns number of created disks, which can be < num if the packing is too tight. The computation is done according to the given psd.")

		.def("makeClumpCloud",&DiskPack::makeClumpCloud,(boost::python::arg("minCorner"),boost::python::arg("maxCorner"),boost::python::arg("clumps"),boost::python::arg("periodic")=false,boost::python::arg("num")=-1,boost::python::arg("seed")=0),"Create random loose packing of clumps within box given by *minCorner* and *maxCorner*. Clumps are selected with equal probability. At most *num* clumps will be positioned if *num* is positive; otherwise, as many clumps as possible will be put in space, until maximum number of attempts to place a new clump randomly is attained.\n:param seed: number used to initialize the random number generator.")
		//
		.def("aabb",&DiskPack::aabb_py,"Get axis-aligned bounding box coordinates, as 2 3-tuples.")
		.def("dim",&DiskPack::dim,"Return dimensions of the packing in terms of aabb(), as a 3-tuple.")
		.def("center",&DiskPack::midPt,"Return coordinates of the bounding box center.")
		.def_readwrite("cellSize",&DiskPack::cellSize,"Size of periodic cell; is Vector3(0,0,0) if not periodic. (Change this property only if you know what you're doing).")
		.def_readwrite("isPeriodic",&DiskPack::isPeriodic,"was the packing generated in periodic boundaries?")
		.def("cellFill",&DiskPack::cellFill,"Repeat the packing (if periodic) so that the results has dim() >= given size. The packing retains periodicity, but changes cellSize. Raises exception for non-periodic packing.")
		.def("cellRepeat",&DiskPack::cellRepeat,"Repeat the packing given number of times in each dimension. Periodicity is retained, cellSize changes. Raises exception for non-periodic packing.")
		.def("relDensity",&DiskPack::relDensity,"Relative packing density, measured as sum of disks' volumes / aabb volume.\n(Disk overlaps are ignored.)")
		.def("translate",&DiskPack::translate,"Translate all disks by given vector.")
		.def("rotate",&DiskPack::rotate,(boost::python::arg("axis"),boost::python::arg("angle")),"Rotate all disks around packing center (in terms of aabb()), given axis and angle of the rotation.")
		.def("scale",&DiskPack::scale,"Scale the packing around its center (in terms of aabb()) by given factor (may be negative).")
		.def("hasClumps",&DiskPack::hasClumps,"Whether this object contains clumps.")
		.def("getClumps",&DiskPack::getClumps,"Return lists of disk ids sorted by clumps they belong to. The return value is (standalones,[clump1,clump2,…]), where each item is list of id's of disks.")
		.def("__len__",&DiskPack::len,"Get number of disks in the packing")
		.def("__getitem__",&DiskPack::getitem,"Get entry at given index, as tuple of center and radius.")
		.def("__iter__",&DiskPack::getIterator,"Return iterator over disks.")
		.def_readonly("appliedPsdScaling",&DiskPack::appliedPsdScaling,"A factor between 0 and 1, uniformly applied on all sizes of of the PSD.")
		;
  boost::python::class_<DiskPack::_iterator>("DiskPackIterator",boost::python::init<DiskPack::_iterator&>())
		.def("__iter__",&DiskPack::_iterator::iter)
		.def("next",&DiskPack::_iterator::next)
	;
}

