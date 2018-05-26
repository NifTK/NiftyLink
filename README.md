NiftyLink
---------

NiftyLink is an open-source software library to facilitate
communication over TCP/IP using the [OpenIGTLink][openigtlink] protocol.

NiftyLink was developed by the [Centre for Medical Image Computing][cmic] at [University College London (UCL)][ucl].

If you use this software, please cite [this paper][citation]. 

Useful Links
------------------

 - [Mailing list][mailinglist].
 - NiftyLink is part of [NifTK][niftk].
 - [NiftyLink home page][NiftyLinkHome].
 - [Dashboard][dashboard].
 - [Doxygen][doxygen].

License
-----------

Copyright (c) 2014, [University College London][ucl].

NiftyLink is available as free open-source software under a BSD license.
Other licenses apply for the dependencies:

 - [Qt project][qt] uses [LGPL][qt-lgpl]
 - [QsLog][qslog] uses [BSD][qslog-bsd]


Funding
-------------

This project was funded by the [NIHR Biomedical
Research Centre][nihr] at [University College London][ucl] (UCL) and
[University College London Hospitals][uclh] (UCLH), 
[Innovate UK][innovateuk] and the [Engineering And
Physical Science Research Council][epsrc] (EPSRC).


Supported Platforms
-----------------------------

NiftyLink is a cross-platform C++ library and officially supports:

 - Windows
 - MacOS X
 - Linux


Branching Workflow
------------------

 1. Raise issue in Gitlab Issue Tracker.
 2. Create a feature branch called <issue-number>-some-short-description
    replacing <issue-number> with the Gitlab issue number.
 3. Code on that branch.
 4. Push to remote when ready.
 5. Merge to master when Gitlab Pipelines are Green.
 6. Remove merged (feature) branch.

An alternative to points 5 and 6 is to assign a merge request, 
and the merger should select to automatically delete the branch
via the web interface.

[openigtlink]: http://openigtlink.org
[cmic]: http://www.ucl.ac.uk/medical-image-computing
[ucl]: http://www.ucl.ac.uk
[qt]: http://qt-project.org
[qt-lgpl]: http://qt-project.org/products/licensing
[qslog]: https://bitbucket.org/razvanpetru/qt-components/wiki/QsLog
[qslog-bsd]: http://opensource.org/licenses/BSD-3-Clause
[nihr]: http://www.nihr.ac.uk/research
[uclh]: http://www.uclh.nhs.uk
[innovateuk]: https://www.innovateuk.org
[epsrc]: http://www.epsrc.ac.uk
[mailinglist]: https://www.mailinglists.ucl.ac.uk/mailman/listinfo/niftk-users
[api]: http://cmic.cs.ucl.ac.uk/platform/niftk/current/NiftyLink/index.html
[dashboard]: http://cdash.cmiclab.cs.ucl.ac.uk/index.php?project=NiftyLink
[citation]: https://doi.org/10.1007/s11548-014-1124-7
[niftk]: http://www.niftk.org
[NiftyLinkHome]: https://cmiclab.cs.ucl.ac.uk/CMIC/NiftyLink
[doxygen]: http://cmic.cs.ucl.ac.uk/NiftyLink-API
