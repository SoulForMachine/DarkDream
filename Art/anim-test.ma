//Maya ASCII 2008 scene
//Name: anim-test.ma
//Last modified: Thu, Nov 13, 2008 09:02:12 PM
//Codeset: 1252
requires maya "2008";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya Unlimited 2008";
fileInfo "version" "2008";
fileInfo "cutIdentifier" "200708022245-704165";
fileInfo "osv" "Microsoft Windows XP Service Pack 3 (Build 2600)\n";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -7.6416944316845647 12.969260988399705 15.161780646316659 ;
	setAttr ".r" -type "double3" -22.538352729616292 332.99999999996663 0 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 15.413402659112904;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 0.49482239402269279 8.9559430269928733 1.2800603918007614 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 2.2251196270703257 100.1 1.367901410084216 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 18.858800773694387;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1.7651242169886063 4.5390779656461957 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 35.946268721909924;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 4.5977200707848072 2.4198526688341095 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 9.6354133540849123;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pCube1";
	setAttr ".t" -type "double3" 0.49482263244127189 7.209991851303176 1.099376778977275 ;
	setAttr -l on ".tx";
	setAttr -l on ".ty";
	setAttr -l on ".tz";
	setAttr -l on ".rx";
	setAttr -l on ".ry";
	setAttr -l on ".rz";
	setAttr -l on ".sx";
	setAttr -l on ".sy";
	setAttr -l on ".sz";
createNode mesh -n "pCubeShape1" -p "pCube1";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -max 12 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr -s 4 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode mesh -n "pCubeShape1Orig" -p "pCube1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode joint -n "joint8";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".t" -type "double3" 0.51371362083737093 6.2036222964656247 1.0119383887851503 ;
	setAttr -av ".tx";
	setAttr -av ".ty";
	setAttr -av ".tz";
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -90 -14.036243467926505 90 ;
	setAttr ".bps" -type "matrix" 4.4408920985006262e-016 0.97014250014533165 0.24253562503633325 0
		 -2.2204460492503126e-016 0.24253562503633352 -0.97014250014533165 0 -0.99999999999999978 2.2204460492503126e-016 4.4408920985006262e-016 0
		 0.51371362083737093 6.2036222964656247 1.0119383887851503 1;
	setAttr ".radi" 0.5;
createNode joint -n "joint9" -p "joint8";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 1;
	setAttr ".t" -type "double3" 0.90702801382143683 4.4408920985006262e-016 0 ;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -1.3866916220176992e-031 -1.1264242515808829e-030 
		14.036243467926509 ;
	setAttr ".bps" -type "matrix" 3.7697608929006648e-016 0.99999999999999989 -1.6653345369377348e-016 0
		 -3.2312236224862867e-016 8.3266726846886741e-017 -0.99999999999999989 0 -0.99999999999999978 2.2204460492503126e-016 4.4408920985006262e-016 0
		 0.51371362083737138 7.0835687214962082 1.231924995042796 1;
	setAttr ".radi" 0.51313398218975437;
createNode joint -n "joint10" -p "joint9";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 2;
	setAttr ".t" -type "double3" 1.2539236556685847 2.2204460492503131e-016 2.4651903288156619e-032 ;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 180 1.4124500153760508e-030 17.818888914522663 ;
	setAttr ".bps" -type "matrix" 2.6001359468091385e-016 0.95202856128526481 -0.30600918041311886 0
		 3.0051518185491874e-016 0.30600918041311864 0.95202856128526492 0 0.99999999999999978 -1.8456928859476819e-016 -3.2749933682257666e-016 0
		 0.51371362083737182 8.3374923771647929 1.2319249950427955 1;
	setAttr ".radi" 0.5;
createNode joint -n "joint11" -p "joint10";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 3;
	setAttr ".t" -type "double3" 0.64700001929547479 4.4408920985006262e-016 0 ;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -2.6170802103094879e-016 -1.8984644113246301e-015 
		15.697792517861005 ;
	setAttr ".bps" -type "matrix" 3.3162404811870071e-016 0.99931483376676689 -0.037011660509883915 0
		 2.1895646748509423e-016 0.037011660509883748 0.999314833766767 0 0.99999999999999978 -1.8456928859476819e-016 -3.2749933682257666e-016 0
		 0.51371362083737204 8.9534548746862015 1.0339370494109157 1;
	setAttr ".radi" 0.5;
createNode joint -n "joint12" -p "joint11";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 4;
	setAttr ".t" -type "double3" 0.59437107988959426 2.2204460492503131e-016 1.9694161627268558e-017 ;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 3.5338751649251727e-018 -1.8729605050830606e-015 -0.21620946246150444 ;
	setAttr ".bps" -type "matrix" 3.3079544205226375e-016 0.99916805310057788 -0.040782369514297902 0
		 2.2020631113034171e-016 0.040782369514297735 0.99916805310057799 0 0.99999999999999978 -1.8456928859476819e-016 -3.2749933682257666e-016 0
		 0.51371362083737226 9.5474187115818445 1.0119383887851492 1;
	setAttr ".radi" 0.50407751214747931;
createNode joint -n "joint13" -p "joint12";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 5;
	setAttr ".t" -type "double3" 1.0788319015179333 -1.1102230246251565e-014 3.5266289425573731e-017 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -87.662694140876866 -90 0 ;
	setAttr ".bps" -type "matrix" 0.99999999999999978 -6.9111598229198479e-017 -2.2109715158371585e-016 0
		 2.2847847049728766e-016 0.99999999999999989 -5.2735593669694936e-016 0 2.0653248922114291e-016 4.2327252813834093e-016 1 0
		 0.51371362083737271 10.625353082244311 0.96794106753362108 1;
	setAttr ".radi" 0.50407751214747931;
createNode joint -n "joint4" -p "joint11";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 4;
	setAttr ".r" -type "double3" 0 0 3.1283743759969207 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -180 -86.467705416109169 92.1210963966615 ;
	setAttr ".bps" -type "matrix" 0.99661282778912574 0.054573307938201546 0.061519310357352121 0
		 -0.054469631208033768 0.99850976663259638 -0.0033623229140789762 0 -0.061611125312095671 2.6065178416296801e-015 0.99810023005596848 0
		 0.91193427338947963 8.8556638631248337 1.1855145554062969 1;
	setAttr ".radi" 0.60139435780133876;
createNode joint -n "joint5" -p "joint4";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 5;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 0 -24.049734623906335 0 ;
	setAttr ".bps" -type "matrix" 0.88499056427556533 0.049835911099450549 0.46293421034538373 0
		 -0.054469631208033768 0.99850976663259638 -0.0033623229140789762 0 -0.46241189476404809 -0.022240231658088312 0.88638626550534816 0
		 3.8621981757501946 9.0172167309518159 1.3676296111075756 1;
	setAttr ".radi" 0.59089798895892343;
createNode joint -n "joint6" -p "joint5";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 6;
	setAttr ".t" -type "double3" 2.7573611198725185 0 1.1102230246251565e-016 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 0 27.582029207797163 0 ;
	setAttr ".bps" -type "matrix" 0.99851542345527855 0.054469631208034018 -9.1640527452252485e-005 0
		 -0.054469631208033768 0.99850976663259638 -0.0033623229140789762 0 -9.1640527451808396e-005 0.0033623229140787303 0.99999434317731783 0
		 6.30243674913768 9.1546323345908647 2.644106403772823 1;
	setAttr ".radi" 0.59089798895892343;
createNode joint -n "joint17" -p "joint11";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 4;
	setAttr ".r" -type "double3" 0 0 3.1283743759969207 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 179.9999999999998 -86.467705416109183 -87.87890360333833 ;
	setAttr ".bps" -type "matrix" 0.99661282778912574 -0.054573307938201802 -0.061519310357352516 0
		 -0.054469631208034178 -0.99850976663259638 0.0033623229140795669 0 -0.061611125312095866 -3.3054197644992784e-015 -0.9981002300559686 0
		 0.11549300000000107 8.8556599999999968 1.1855099999999994 1;
	setAttr ".radi" 0.60139435780133876;
createNode joint -n "joint18" -p "joint17";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 5;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 8.5377363811685451e-007 -24.04973462390635 3.8189431178099322e-014 ;
	setAttr ".bps" -type "matrix" 0.88499056427556499 -0.049835911099451055 -0.46293421034538451 0
		 -0.054469631208034178 -0.99850976663259638 0.0033623229140795669 0 -0.46241189476404859 0.022240231658087799 -0.88638626550534783 0
		 -2.8347699999999971 9.0172199999999965 1.3676300000000001 1;
	setAttr ".radi" 0.59089798895892343;
createNode joint -n "joint19" -p "joint18";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 6;
	setAttr ".t" -type "double3" -2.757363587933658 5.6837794755182358e-006 -2.3078811057231086e-006 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 8.5377364402811873e-007 27.582029207797174 -9.058500666388186e-015 ;
	setAttr ".bps" -type "matrix" 0.99851542345527855 -0.054469631208034233 9.1640527451641862e-005 0
		 -0.054469631208034178 -0.99850976663259638 0.0033623229140795669 0 -9.1640527452196974e-005 -0.0033623229140794277 -0.99999434317731795 0
		 -5.2750099999999982 9.1546299999999938 2.6441100000000008 1;
	setAttr ".radi" 0.59089798895892343;
createNode joint -n "joint1" -p "joint8";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 1;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 1.4202075594042713e-015 1.4163051826339494e-014 -159.56935876684588 ;
	setAttr ".bps" -type "matrix" -3.3864414341567834e-016 -0.99377881355723696 0.11137176358831158 0
		 3.6309664968411341e-016 0.1113717635883113 0.99377881355723696 0 -0.99999999999999978 2.2204460492503126e-016 4.4408920985006262e-016 0
		 0 5.4996651564411554 1.099933031288211 1;
	setAttr ".radi" 0.58109411292513113;
createNode joint -n "joint2" -p "joint1";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 2;
	setAttr ".t" -type "double3" 2.5678195165525359 -8.8817841970012523e-016 5.7017047007169253e-016 ;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 180 -8.6511087335807553e-015 -16.260204708312031 ;
	setAttr ".bps" -type "matrix" -4.2676543959060329e-016 -0.98521175481967438 -0.17134117475124844 0
		 -3.7621710345509372e-016 0.17134117475124871 -0.9852117548196746 0 0.99999999999999978 -2.0106136280290485e-016 -5.6474285205228876e-016 0
		 -1.4397475107006876e-015 2.9478205238524584 1.3859156194231517 1;
	setAttr ".radi" 0.60765776309636432;
createNode joint -n "joint3" -p "joint2";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 3;
	setAttr ".t" -type "double3" 3.0813834198630432 -4.4408920985006262e-016 -4.6525910357850031e-016 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 80.134193056915592 -89.999999999999972 0 ;
	setAttr ".bps" -type "matrix" 0.99999999999999978 -6.4809461839952557e-016 -5.8614338024574435e-016 0
		 8.0008205701800283e-016 1 8.3266726846886494e-017 0 4.4377600445085471e-016 -3.8857805861880479e-016 1.0000000000000002 0
		 -3.2200345640042362e-015 -0.087994642503059772 0.85794776440479892 1;
	setAttr ".radi" 0.60765776309636432;
createNode joint -n "joint14" -p "joint8";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 1;
	setAttr ".t" -type "double3" -0.66159294045226158 -0.25609791191484299 -0.51371637916262936 ;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 8.5377364570377955e-007 -3.0399605998612391e-015 20.430641233154109 ;
	setAttr ".bps" -type "matrix" 3.3864414341567829e-016 0.99377881355723674 -0.11137176358831138 0
		 -3.6309664968411331e-016 -0.11137176358831113 -0.99377881355723663 0 -0.99999999999999978 2.2204460492503126e-016 4.4408920985006262e-016 0
		 1.0274299999999998 5.4996700000000001 1.0999299999999999 1;
	setAttr ".radi" 0.58109411292513113;
createNode joint -n "joint15" -p "joint14";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 2;
	setAttr ".r" -type "double3" 0 0 0 ;
	setAttr -av ".rx";
	setAttr -av ".ry";
	setAttr -av ".rz";
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" -180 -1.0838121078422283e-014 -16.260204708311996 ;
	setAttr ".bps" -type "matrix" 4.2676543959060304e-016 0.98521175481967427 0.17134117475124794 0
		 3.7621710345509387e-016 -0.17134117475124819 0.98521175481967416 0 0.99999999999999978 -2.0106136280290492e-016 -5.6474285205228876e-016 0
		 1.0274299999999987 2.9478200000000023 1.3859199999999998 1;
	setAttr ".radi" 0.60765776309636432;
createNode joint -n "joint16" -p "joint15";
	addAttr -ci true -sn "liw" -ln "lockInfluenceWeights" -bt "lock" -min 0 -max 1 
		-at "bool";
	setAttr ".uoc" yes;
	setAttr ".oc" 3;
	setAttr ".t" -type "double3" -3.0813835720889537 -4.1807246624614436e-006 2.2204460492503131e-016 ;
	setAttr ".mnrl" -type "double3" -360 -360 -360 ;
	setAttr ".mxrl" -type "double3" 360 360 360 ;
	setAttr ".jo" -type "double3" 80.13419305691562 -89.999999999999943 0 ;
	setAttr ".bps" -type "matrix" 0.99999999999999978 6.6447111079906268e-016 -3.5787107951555454e-016 0
		 5.8769672376344255e-016 -1 -1.3877787807814511e-016 0 -2.2173139952582355e-016 2.4980018054066017e-016 -0.99999999999999989 0
		 1.0274299999999976 -0.087994599999996925 0.85794800000000071 1;
	setAttr ".radi" 0.60765776309636432;
createNode lightLinker -n "lightLinker1";
	setAttr -s 2 ".lnk";
	setAttr -s 2 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode polyCube -n "polyCube1";
	setAttr ".w" 2.5889636354456549;
	setAttr ".h" 4.496567761161983;
	setAttr ".d" 1.1397935049640004;
	setAttr ".sw" 3;
	setAttr ".sh" 2;
	setAttr ".cuv" 4;
createNode polyExtrudeFace -n "polyExtrudeFace1";
	setAttr ".ics" -type "componentList" 1 "f[7]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482265 9.4582758 1.0993768 ;
	setAttr ".rs" 41949;
	setAttr ".lt" -type "double3" -5.5511151231257827e-017 5.6288233407497989e-017 0.25349966699935145 ;
	setAttr ".c[0]"  0 1 1;
createNode polyTweak -n "polyTweak1";
	setAttr ".uopa" yes;
	setAttr -s 17 ".tk";
	setAttr ".tk[1:2]" -type "float3" 0.23888838 0 0  -0.23888838 0 0 ;
	setAttr ".tk[4:10]" -type "float3" 0 1.1067305 0  0.23888838 1.1067305 
		0  -0.23888838 1.1067305 0  0 1.1067305 0  0 0 0  -0.085745759 0 0  0.085745759 0 
		0 ;
	setAttr ".tk[13:14]" -type "float3" -0.085745759 0 0  0.085745759 0 0 ;
	setAttr ".tk[16:19]" -type "float3" 0 1.1067305 0  0.23888838 1.1067305 
		0  -0.23888838 1.1067305 0  0 1.1067305 0 ;
	setAttr ".tk[21:22]" -type "float3" 0.23888838 0 0  -0.23888838 0 0 ;
createNode polyExtrudeFace -n "polyExtrudeFace2";
	setAttr ".ics" -type "componentList" 1 "f[7]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482259 9.7117758 1.0993768 ;
	setAttr ".rs" 39607;
	setAttr ".lt" -type "double3" 0 -6.466143640220922e-018 1.0817025672857292 ;
	setAttr ".c[0]"  0 1 1;
createNode polyExtrudeFace -n "polyExtrudeFace3";
	setAttr ".ics" -type "componentList" 1 "f[26]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482259 10.252627 1.6692739 ;
	setAttr ".rs" 62633;
	setAttr ".lt" -type "double3" 5.5511151231257827e-017 7.8009124521852334e-016 0.27645496502133254 ;
	setAttr ".ls" -type "double3" 0.63333332949421994 0.63333332949421994 0.63333332949421994 ;
	setAttr ".c[0]"  0 1 1;
createNode polyExtrudeFace -n "polyExtrudeFace4";
	setAttr ".ics" -type "componentList" 2 "f[15]" "f[17]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482268 5.3328767 1.035521 ;
	setAttr ".rs" 61495;
	setAttr ".lt" -type "double3" -4.3368086899420177e-019 2.0072171257132321e-016 2.096029770058621 ;
	setAttr ".c[0]"  0 1 1;
createNode polyTweak -n "polyTweak2";
	setAttr ".uopa" yes;
	setAttr -s 36 ".tk[0:35]" -type "float3"  0.42766276 0.37116864 -0.10281638 
		0.063631825 0.37116864 -0.02489505 -0.063631803 0.37116864 -0.02489505 -0.42766276 
		0.37116864 -0.10281638 0 0.15537716 0.24656987 0 0.15537716 0.3244912 0 0.15537716 
		0.3244912 0 0.15537716 0.24656987 0.068753533 -0.19381478 -0.1390692 0 0 -0.061147869 
		0 0 -0.061147869 -0.068753533 -0.19381478 -0.1390692 0.068753533 -0.19381478 -0.1390692 
		0 0 -0.067731313 0 0 -0.067731313 -0.068753533 -0.19381478 -0.1390692 0 0 -0.14162228 
		0 0 -0.070284396 0 0 -0.070284396 0 0 -0.14162228 0.42766276 0.37116864 -0.10281638 
		0.063631825 0.37116864 -0.031478494 -0.063631803 0.37116864 -0.031478494 -0.42766276 
		0.37116864 -0.10281638 0 0 -0.1390692 0 0 -0.1390692 0 0 -0.1390692 0 0 -0.1390692 
		0 0 -0.1390692 0 0 -0.1390692 0 0 -0.1390692 0 0 -0.1390692 0 0 -0.1390692 0 0 -0.1390692 
		0 0 -0.1390692 0 0 -0.1390692;
createNode polyExtrudeFace -n "polyExtrudeFace5";
	setAttr ".ics" -type "componentList" 2 "f[15]" "f[17]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482268 3.2368464 1.0355211 ;
	setAttr ".rs" 60241;
	setAttr ".lt" -type "double3" 0 -3.318381023771237e-017 0.14944659542129468 ;
	setAttr ".c[0]"  0 1 1;
createNode polyExtrudeFace -n "polyExtrudeFace6";
	setAttr ".ics" -type "componentList" 2 "f[15]" "f[17]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482268 3.0874 1.0355211 ;
	setAttr ".rs" 62037;
	setAttr ".lt" -type "double3" 0 1.837591319453171e-016 0.1724224418451441 ;
	setAttr ".c[0]"  0 1 1;
createNode polyExtrudeFace -n "polyExtrudeFace7";
	setAttr ".ics" -type "componentList" 2 "f[15]" "f[17]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482268 2.9149773 1.035521 ;
	setAttr ".rs" 59657;
	setAttr ".lt" -type "double3" 8.6736173798840355e-019 2.1230578612311671e-017 2.9043859740727331 ;
	setAttr ".ls" -type "double3" 0.65000000988329176 0.65000000988329176 0.65000000988329176 ;
	setAttr ".c[0]"  0 1 1;
createNode polyExtrudeFace -n "polyExtrudeFace8";
	setAttr ".ics" -type "componentList" 2 "f[19]" "f[21]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482268 8.7905922 1.1518506 ;
	setAttr ".rs" 37877;
	setAttr ".lt" -type "double3" 2.9056618222611519e-015 -0.17204480775220485 1.9979439266335142 ;
	setAttr ".ls" -type "double3" 0.63333332581517188 0.64872273030260563 0.63333332581517188 ;
	setAttr ".c[0]"  0 1 1;
createNode polyTweak -n "polyTweak3";
	setAttr ".uopa" yes;
	setAttr -s 41 ".tk";
	setAttr ".tk[0:4]" -type "float3" 0 0 0.081812099  0 0 0.081812099  0 
		0 0.081812099  0 0 0.081812099  0 0 0 ;
	setAttr ".tk[20:23]" -type "float3" 0 0 0.081812099  0 0 0.081812099  0 
		0 0.081812099  0 0 0.081812099 ;
	setAttr ".tk[36:67]" -type "float3" 0 0 0.27806038  0 0 0.26167881  0 0 
		-0.0015682448  0 0 0.016325124  0 0 0.26167881  0 0 0.27806038  0 0 0.016325124  
		0 0 -0.0015682448  0 0 0.27806038  0 0 0.26167881  0 0 -0.0015682448  0 0 0.016325124  
		0 0 0.26167881  0 0 0.27806038  0 0 0.016325124  0 0 -0.0015682448  0 0 0.27806038  
		0 0 0.26167881  0 0 -0.0015682448  0 0 0.016325124  0 0 0.26167881  0 0 0.27806038  
		0 0 0.016325124  0 0 -0.0015682448  0 0 -0.20796707  0 0 -0.20796707  0 0 -0.20796707  
		0 0 -0.20796707  0 0 -0.20796707  0 0 -0.20796707  0 0 -0.20796707  0 0 -0.20796707 ;
createNode polyExtrudeFace -n "polyExtrudeFace9";
	setAttr ".ics" -type "componentList" 2 "f[19]" "f[21]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482238 8.9559431 1.2800604 ;
	setAttr ".rs" 52527;
	setAttr ".lt" -type "double3" -1.416401718135063e-014 -0.10588853002963206 0.15388959965567697 ;
	setAttr ".c[0]"  0 1 1;
createNode polyExtrudeFace -n "polyExtrudeFace10";
	setAttr ".ics" -type "componentList" 2 "f[19]" "f[21]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.49482262 8.9740705 1.3850725 ;
	setAttr ".rs" 45910;
	setAttr ".lt" -type "double3" 7.0776717819853729e-015 -0.13621929416314782 0.15436883744463126 ;
	setAttr ".c[0]"  0 1 1;
createNode polyExtrudeFace -n "polyExtrudeFace11";
	setAttr ".ics" -type "componentList" 2 "f[19]" "f[21]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.4948231 8.9939985 1.520362 ;
	setAttr ".rs" 35472;
	setAttr ".lt" -type "double3" 2.4147350785597155e-015 -1.0501419446674467 2.2316221595837069 ;
	setAttr ".c[0]"  0 1 1;
createNode tweak -n "tweak1";
createNode objectSet -n "tweakSet1";
	setAttr ".ihi" 0;
	setAttr ".vo" yes;
createNode groupId -n "groupId2";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts2";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "vtx[*]";
createNode animCurveTA -n "joint14_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  1 0 9 12.371582652028813 10 0;
createNode animCurveTA -n "joint14_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  1 0 9 -7.6004306064261833 10 0;
createNode animCurveTA -n "joint14_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  1 0 9 85.394633801850759 10 108.56451731259713;
createNode animCurveTA -n "joint15_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 0;
createNode animCurveTA -n "joint15_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 0;
createNode animCurveTA -n "joint15_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  6 0 10 109.17792771944916;
createNode animCurveTU -n "joint15_visibility";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 1;
	setAttr ".kot[0]"  5;
createNode animCurveTL -n "joint15_translateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 -2.5678256760446541;
createNode animCurveTL -n "joint15_translateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 -6.7679764019779043e-006;
createNode animCurveTL -n "joint15_translateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 2.2204460492503131e-016;
createNode animCurveTU -n "joint15_scaleX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 1;
createNode animCurveTU -n "joint15_scaleY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 1;
createNode animCurveTU -n "joint15_scaleZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  6 1;
createNode animCurveTA -n "joint8_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  1 0 5 0;
createNode animCurveTA -n "joint8_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  1 0 5 0;
createNode animCurveTA -n "joint8_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  1 0 5 0 10 -17.401601304365201;
createNode animCurveTL -n "joint8_translateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  1 0.51371362083737093;
createNode animCurveTL -n "joint8_translateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  1 6.2036222964656247 10 8.2579816895376013;
createNode animCurveTL -n "joint8_translateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  1 1.0119383887851503;
createNode animCurveTL -n "joint1_translateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  1 -0.66159690419916739;
createNode animCurveTL -n "joint1_translateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  1 -0.25610202743193788;
createNode animCurveTL -n "joint1_translateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  1 0.51371362083737082;
createNode animCurveTA -n "joint1_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  1 0;
createNode animCurveTA -n "joint1_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  1 0 9 -11.223224473210937;
createNode animCurveTA -n "joint1_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  1 0 7 55.7270213486972;
createNode animCurveTA -n "joint2_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  4 0;
createNode animCurveTA -n "joint2_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  4 0 9 -21.214294911831107;
createNode animCurveTA -n "joint2_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  4 0 9 95.466945791285767;
createNode animCurveTA -n "joint12_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  5 0 10 0.14355172258630491 16 -2.3185633479373737;
createNode animCurveTA -n "joint9_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  5 0;
createNode animCurveTA -n "joint10_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  5 0;
createNode animCurveTA -n "joint11_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  5 0 12 -6.6356279891081558 20 -4.9504646854312107;
createNode animCurveTA -n "joint12_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  5 0 10 3.077295023585342 16 2.0290006270771173;
createNode animCurveTA -n "joint9_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  5 0;
createNode animCurveTA -n "joint10_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  5 0;
createNode animCurveTA -n "joint11_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  5 0 12 5.2032489431807365 20 6.825917463840355;
createNode animCurveTA -n "joint12_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  5 0 10 -3.8405882224071637 16 -55.344619655496153;
createNode animCurveTA -n "joint9_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  5 0 10 -17.401601304365201;
createNode animCurveTA -n "joint10_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  5 0 10 -17.401601304365201;
createNode animCurveTA -n "joint11_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 4 ".ktv[0:3]"  5 0 10 31.456199420052318 12 36.089558295000657 
		20 52.067377016137208;
createNode animCurveTL -n "joint17_translateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  10 -0.10333783552997033;
createNode animCurveTL -n "joint4_translateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  10 -0.10333414365516802;
createNode animCurveTL -n "joint17_translateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  10 0.14784954721997612;
createNode animCurveTL -n "joint4_translateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  10 0.14785424248572809;
createNode animCurveTL -n "joint17_translateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  10 -0.39822062083737109;
createNode animCurveTL -n "joint4_translateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  10 0.3982206525521077;
createNode animCurveTA -n "joint17_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  10 0 20 -74.089040297821185;
createNode animCurveTA -n "joint4_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  10 0 20 -74.089040297821199;
createNode animCurveTA -n "joint17_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  10 0 20 -26.941328086255922;
createNode animCurveTA -n "joint4_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  10 0 20 -26.941328086255911;
createNode animCurveTA -n "joint17_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  10 -73.576437605835764 20 4.4126380505771303;
createNode animCurveTA -n "joint4_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  10 -76.473505044349139 20 1.5155706120636834;
createNode animCurveTA -n "joint18_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  8 0 15 -59.910129217749514;
createNode animCurveTA -n "joint18_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  8 0 15 -74.468000451693797;
createNode animCurveTA -n "joint18_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  8 0 15 78.036958240848477;
createNode animCurveTA -n "joint5_rotateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  8 0 15 -59.910129217749514;
createNode animCurveTA -n "joint5_rotateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  8 0 15 -74.468000451693797;
createNode animCurveTA -n "joint5_rotateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  8 0 15 78.036958240848477;
createNode animCurveTU -n "joint18_visibility";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
	setAttr ".kot[0]"  5;
createNode animCurveTL -n "joint18_translateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 -2.9602907115844057;
createNode animCurveTL -n "joint18_translateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 -7.1540713406648138e-006;
createNode animCurveTL -n "joint18_translateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 -4.9905011538786681e-006;
createNode animCurveTU -n "joint18_scaleX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
createNode animCurveTU -n "joint18_scaleY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
createNode animCurveTU -n "joint18_scaleZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
createNode animCurveTU -n "joint5_visibility";
	setAttr ".tan" 9;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
	setAttr ".kot[0]"  5;
createNode animCurveTL -n "joint5_translateX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 2.9602909174925496;
createNode animCurveTL -n "joint5_translateY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 0;
createNode animCurveTL -n "joint5_translateZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 3.9968028886505635e-015;
createNode animCurveTU -n "joint5_scaleX";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
createNode animCurveTU -n "joint5_scaleY";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
createNode animCurveTU -n "joint5_scaleZ";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr ".ktv[0]"  8 1;
createNode polySmoothFace -n "polySmoothFace1";
	setAttr ".ics" -type "componentList" 1 "f[*]";
	setAttr ".suv" yes;
	setAttr ".ps" 0.10000000149011612;
	setAttr ".ro" 1;
	setAttr ".ma" yes;
	setAttr ".m08" yes;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n"
		+ "                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n"
		+ "                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n"
		+ "            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n"
		+ "            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n"
		+ "                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n"
		+ "                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n"
		+ "            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n"
		+ "                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n"
		+ "                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nRigids 1\n"
		+ "                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n"
		+ "            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n"
		+ "            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 1\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n"
		+ "                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n"
		+ "                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n"
		+ "            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n"
		+ "            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nRigids 1\n"
		+ "            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -autoExpand 0\n                -showDagOnly 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUnitlessCurves 1\n                -showCompounds 1\n"
		+ "                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -autoExpand 0\n            -showDagOnly 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n"
		+ "            -attrAlphaOrder \"default\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -autoExpand 1\n                -showDagOnly 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n"
		+ "                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n"
		+ "                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -autoExpand 1\n                -showDagOnly 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n"
		+ "                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n"
		+ "                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -autoExpand 0\n"
		+ "                -showDagOnly 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n"
		+ "                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n"
		+ "                -showMuteInfo 0\n                -autoExpand 0\n                -showDagOnly 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -sortOrder \"none\" \n"
		+ "                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -zoom 1\n                -animateTransition 0\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n"
		+ "                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -zoom 1\n                -animateTransition 0\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"multiListerPanel\" (localizedPanelLabel(\"Multilister\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"multiListerPanel\" -l (localizedPanelLabel(\"Multilister\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Multilister\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"devicePanel\" (localizedPanelLabel(\"Devices\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tdevicePanel -unParent -l (localizedPanelLabel(\"Devices\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tdevicePanel -edit -l (localizedPanelLabel(\"Devices\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"webBrowserPanel\" (localizedPanelLabel(\"Web Browser\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"webBrowserPanel\" -l (localizedPanelLabel(\"Web Browser\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Web Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -zoom 1\n                -animateTransition 0\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -currentNode \"joint19\" \n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n"
		+ "                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -zoom 1\n                -animateTransition 0\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -currentNode \"joint19\" \n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n"
		+ "                -graphType \"DAG\" \n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"vacantCell.xpm\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 50 -ast 1 -aet 50 ";
	setAttr ".st" 6;
createNode polyTriangulate -n "polyTriangulate1";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "polyTweak4";
	setAttr ".uopa" yes;
	setAttr -s 30 ".tk";
	setAttr ".tk[6:7]" -type "float3" -3.7252903e-009 0 0  0 0 0 ;
	setAttr ".tk[16:19]" -type "float3" 1.4901161e-008 0 0  -3.7252903e-009 
		0 0  0 0 0  1.4901161e-008 0 0 ;
	setAttr ".tk[103:105]" -type "float3" 0 0 0  0 0 0  0 0 0 ;
	setAttr ".tk[112:114]" -type "float3" 0 0 0  -9.3132257e-010 0 0  -1.4901161e-008 
		0 0 ;
	setAttr ".tk[118:121]" -type "float3" 0.052182209 0 0  0.016301343 0 0  -0.016233953 
		0 0  -0.052085981 0 0 ;
	setAttr ".tk[134:137]" -type "float3" 0.055034235 0 0  0.017119557 0 0  -0.015263217 
		0 0  -0.054855593 0 0 ;
	setAttr ".tk[142:143]" -type "float3" 1.4901161e-008 0 0  -1.4901161e-008 
		0 0 ;
	setAttr ".tk[296:298]" -type "float3" 0.035789493 0 0  2.8319977e-005 0 0  
		-0.035689868 0 0 ;
	setAttr ".tk[308:310]" -type "float3" 0.03806255 0 0  0.0010709551 0 0  -0.037062898 
		0 0 ;
	setAttr ".tk[314]" -type "float3" -0.059168957 0 0 ;
	setAttr ".tk[316]" -type "float3" 0.059168942 0 0 ;
createNode skinCluster -n "skinCluster1";
	setAttr -s 394 ".wl";
	setAttr -s 2 ".wl[0].w[12:13]"  0.99793075629488637 0.002069243705113692;
	setAttr -s 2 ".wl[1].w[12:13]"  0.99620310415074476 0.0037968958492552623;
	setAttr -s 2 ".wl[2].w[15:16]"  0.99572063007466027 0.0042793699253397934;
	setAttr -s 2 ".wl[3].w[15:16]"  0.99823322768881284 0.0017667723111871856;
	setAttr -s 2 ".wl[4].w[9:10]"  0.98213277799098331 0.017867222009016744;
	setAttr -s 2 ".wl[5].w[1:2]"  0.5169678416484067 0.4830321583515933;
	setAttr -s 2 ".wl[6].w[1:2]"  0.51767383718222815 0.4823261628177718;
	setAttr -s 2 ".wl[7].w[6:7]"  0.98323852184390426 0.016761478156095782;
	setAttr -s 2 ".wl[8].w[9:10]"  0.9990410850745014 0.00095891492549848975;
	setAttr -s 2 ".wl[9].w";
	setAttr ".wl[9].w[5]" 0.033499020705374263;
	setAttr ".wl[9].w[9]" 0.96650097929462575;
	setAttr -s 2 ".wl[10].w[5:6]"  0.03527795008683593 0.96472204991316401;
	setAttr -s 2 ".wl[11].w[6:7]"  0.99907981966346993 0.00092018033653013703;
	setAttr -s 2 ".wl[12].w";
	setAttr ".wl[12].w[5]" 0.014049163129859064;
	setAttr ".wl[12].w[9]" 0.98595083687014096;
	setAttr -s 2 ".wl[13].w[3:4]"  0.5252177826918033 0.4747822173081967;
	setAttr -s 2 ".wl[14].w[3:4]"  0.52723926730828485 0.47276073269171526;
	setAttr -s 2 ".wl[15].w[5:6]"  0.014646459581906563 0.98535354041809353;
	setAttr -s 2 ".wl[16].w[9:10]"  0.96656400653841468 0.033435993461585316;
	setAttr -s 2 ".wl[17].w[1:2]"  0.54487706481710385 0.45512293518289615;
	setAttr -s 2 ".wl[18].w[1:2]"  0.54630834684698648 0.45369165315301363;
	setAttr -s 2 ".wl[19].w[6:7]"  0.9687128995717994 0.031287100428200612;
	setAttr -s 2 ".wl[20].w[12:13]"  0.99816634748992039 0.0018336525100796661;
	setAttr -s 2 ".wl[21].w[0:1]"  0.99259864153983146 0.0074013584601685658;
	setAttr -s 2 ".wl[22].w[0:1]"  0.99151762267788868 0.0084823773221113195;
	setAttr -s 2 ".wl[23].w[15:16]"  0.99843468577965067 0.00156531422034937;
	setAttr -s 2 ".wl[24].w[4:5]"  0.88521259907034644 0.1147874009296535;
	setAttr -s 2 ".wl[25].w[4:5]"  0.89545914584041819 0.10454085415958179;
	setAttr -s 2 ".wl[26].w[4:5]"  0.88757133819584688 0.11242866180415315;
	setAttr -s 2 ".wl[27].w[4:5]"  0.87686921675651186 0.1231307832434882;
	setAttr -s 2 ".wl[28].w[4:5]"  0.50002487602284662 0.49997512397715344;
	setAttr -s 2 ".wl[29].w[4:5]"  0.50002698439296667 0.49997301560703339;
	setAttr -s 2 ".wl[30].w[4:5]"  0.50962164780963159 0.49037835219036835;
	setAttr -s 2 ".wl[31].w[4:5]"  0.5085895962636473 0.49141040373635264;
	setAttr -s 2 ".wl[32].w[4:5]"  0.72766224769562105 0.27233775230437895;
	setAttr -s 2 ".wl[33].w[4:5]"  0.73173423872896615 0.26826576127103391;
	setAttr -s 2 ".wl[34].w[4:5]"  0.52524747029822316 0.47475252970177684;
	setAttr -s 2 ".wl[35].w[4:5]"  0.52463465986843505 0.47536534013156506;
	setAttr -s 2 ".wl[36].w[12:13]"  0.80111796418233494 0.19888203581766509;
	setAttr -s 2 ".wl[37].w[12:13]"  0.82111030469899959 0.17888969530100049;
	setAttr -s 2 ".wl[38].w[12:13]"  0.91413733850480094 0.085862661495199152;
	setAttr -s 2 ".wl[39].w[12:13]"  0.92844154926489153 0.071558450735108442;
	setAttr -s 2 ".wl[40].w[15:16]"  0.81003829270743233 0.1899617072925677;
	setAttr -s 2 ".wl[41].w[15:16]"  0.81033871850560157 0.18966128149439843;
	setAttr -s 2 ".wl[42].w[15:16]"  0.94676163892856324 0.053238361071436806;
	setAttr -s 2 ".wl[43].w[15:16]"  0.89381943755932747 0.10618056244067252;
	setAttr -s 2 ".wl[44].w[12:13]"  0.54975074665364387 0.45024925334635613;
	setAttr -s 2 ".wl[45].w[12:13]"  0.55396428944567511 0.44603571055432495;
	setAttr -s 2 ".wl[46].w[12:13]"  0.5682682877195252 0.4317317122804748;
	setAttr -s 2 ".wl[47].w[12:13]"  0.57957084855308616 0.4204291514469139;
	setAttr -s 2 ".wl[48].w[15:16]"  0.55075863827491189 0.44924136172508805;
	setAttr -s 2 ".wl[49].w[15:16]"  0.55234430727271833 0.44765569272728162;
	setAttr -s 2 ".wl[50].w[15:16]"  0.59923761836971645 0.40076238163028349;
	setAttr -s 2 ".wl[51].w[15:16]"  0.55653924692791346 0.44346075307208654;
	setAttr -s 2 ".wl[52].w[13:14]"  0.99761093503747567 0.0023890649625244128;
	setAttr -s 2 ".wl[53].w[13:14]"  0.99817170924266452 0.001828290757335468;
	setAttr -s 2 ".wl[54].w[13:14]"  0.99985279429445861 0.00014720570554139993;
	setAttr -s 2 ".wl[55].w[13:14]"  0.99987566176239706 0.00012433823760295089;
	setAttr -s 2 ".wl[56].w[16:17]"  0.99792261472263599 0.0020773852773639974;
	setAttr -s 2 ".wl[57].w[16:17]"  0.997848533128463 0.0021514668715370265;
	setAttr -s 2 ".wl[58].w[16:17]"  0.99992498739644975 7.5012603550197755e-005;
	setAttr -s 2 ".wl[59].w[16:17]"  0.99977788577510673 0.00022211422489328722;
	setAttr -s 2 ".wl[60].w[13:14]"  0.96200019341011933 0.037999806589880711;
	setAttr -s 2 ".wl[61].w[13:14]"  0.97116122089997092 0.02883877910002906;
	setAttr -s 2 ".wl[62].w[13:14]"  0.99542411289090016 0.00457588710909988;
	setAttr -s 2 ".wl[63].w[13:14]"  0.9963771399926874 0.003622860007312617;
	setAttr -s 2 ".wl[64].w[16:17]"  0.96526546814980696 0.034734531850193005;
	setAttr -s 2 ".wl[65].w[16:17]"  0.96697241793241839 0.033027582067581676;
	setAttr -s 2 ".wl[66].w[16:17]"  0.99801034476245709 0.0019896552375429609;
	setAttr -s 2 ".wl[67].w[16:17]"  0.99268405569421569 0.0073159443057843098;
	setAttr -s 2 ".wl[68].w[6:7]"  0.68123413706313451 0.3187658629368656;
	setAttr -s 2 ".wl[69].w[6:7]"  0.75653908472990594 0.24346091527009409;
	setAttr -s 2 ".wl[70].w[6:7]"  0.70005825855478765 0.29994174144521241;
	setAttr -s 2 ".wl[71].w[6:7]"  0.90999336841623968 0.09000663158376028;
	setAttr -s 2 ".wl[72].w[9:10]"  0.65209358123279937 0.34790641876720069;
	setAttr -s 2 ".wl[73].w[9:10]"  0.71826634116318333 0.28173365883681667;
	setAttr -s 2 ".wl[74].w[9:10]"  0.88502544511190706 0.11497455488809294;
	setAttr -s 2 ".wl[75].w[9:10]"  0.67060206235677544 0.32939793764322456;
	setAttr -s 2 ".wl[76].w[6:7]"  0.5 0.5;
	setAttr -s 2 ".wl[77].w[7:8]"  0.99962422983100074 0.0003757701689993122;
	setAttr -s 2 ".wl[78].w[6:7]"  0.5 0.5;
	setAttr -s 2 ".wl[79].w[7:8]"  0.99991278615125134 8.7213848748675298e-005;
	setAttr -s 2 ".wl[80].w[9:10]"  0.5 0.5;
	setAttr -s 2 ".wl[81].w[10:11]"  0.99965864132001647 0.00034135867998353247;
	setAttr -s 2 ".wl[82].w[10:11]"  0.9999250895572267 7.4910442773348936e-005;
	setAttr -s 2 ".wl[83].w[9:10]"  0.5 0.5;
	setAttr -s 2 ".wl[84].w[7:8]"  0.9993724544495235 0.00062754555047654919;
	setAttr -s 2 ".wl[85].w[7:8]"  0.99906197500330274 0.00093802499669723167;
	setAttr -s 2 ".wl[86].w[7:8]"  0.99939159857721538 0.00060840142278461239;
	setAttr -s 2 ".wl[87].w[7:8]"  0.99968461807819464 0.00031538192180538918;
	setAttr -s 2 ".wl[88].w[10:11]"  0.9992293966810023 0.00077060331899774632;
	setAttr -s 2 ".wl[89].w[10:11]"  0.99914283420797656 0.00085716579202344976;
	setAttr -s 2 ".wl[90].w[10:11]"  0.99971973267850578 0.00028026732149418528;
	setAttr -s 2 ".wl[91].w[10:11]"  0.99926306238689111 0.00073693761310887076;
	setAttr -s 2 ".wl[92].w[7:8]"  0.977344463658955 0.022655536341045047;
	setAttr -s 2 ".wl[93].w[7:8]"  0.9894719889106206 0.010528011089379345;
	setAttr -s 2 ".wl[94].w[7:8]"  0.95602630550205103 0.043973694497949016;
	setAttr -s 2 ".wl[95].w[7:8]"  0.97087881204275728 0.029121187957242719;
	setAttr -s 2 ".wl[96].w[10:11]"  0.9685299109157961 0.031470089084203942;
	setAttr -s 2 ".wl[97].w[10:11]"  0.99041747724750095 0.0095825227524990973;
	setAttr -s 2 ".wl[98].w[10:11]"  0.96732773210617695 0.032672267893823055;
	setAttr -s 2 ".wl[99].w[10:11]"  0.94456911400842625 0.055430885991573788;
	setAttr -s 2 ".wl[100].w[12:13]"  0.99678467216968158 0.0032153278303184694;
	setAttr -s 2 ".wl[101].w[0:1]"  0.96379117895462507 0.036208821045374893;
	setAttr -s 2 ".wl[102].w[15:16]"  0.99680321419430051 0.0031967858056994856;
	setAttr -s 2 ".wl[103].w";
	setAttr ".wl[103].w[5]" 0.015326690668071017;
	setAttr ".wl[103].w[9]" 0.98467330933192898;
	setAttr -s 2 ".wl[104].w[1:2]"  0.5191604939218889 0.48083950607811105;
	setAttr -s 2 ".wl[105].w[5:6]"  0.015566310907685057 0.98443368909231499;
	setAttr -s 2 ".wl[106].w";
	setAttr ".wl[106].w[5]" 0.0047815224709194951;
	setAttr ".wl[106].w[9]" 0.99521847752908055;
	setAttr -s 2 ".wl[107].w[3:4]"  0.55006241338865036 0.44993758661134964;
	setAttr -s 2 ".wl[108].w[5:6]"  0.0051350903226142714 0.99486490967738583;
	setAttr -s 2 ".wl[109].w";
	setAttr ".wl[109].w[5]" 0.034103747048059421;
	setAttr ".wl[109].w[9]" 0.96589625295194059;
	setAttr -s 2 ".wl[110].w[3:4]"  0.54755169167936302 0.45244830832063698;
	setAttr -s 2 ".wl[111].w[5:6]"  0.035440573232155631 0.96455942676784434;
	setAttr -s 2 ".wl[112].w[1:2]"  0.52830219640554821 0.47169780359445179;
	setAttr -s 2 ".wl[113].w[1:2]"  0.55191307260413447 0.44808692739586553;
	setAttr -s 2 ".wl[114].w[1:2]"  0.52963497676320626 0.47036502323679374;
	setAttr -s 2 ".wl[115].w[0:1]"  0.99230227922258385 0.0076977207774162146;
	setAttr -s 2 ".wl[116].w[0:1]"  0.99195964526154856 0.0080403547384514749;
	setAttr -s 2 ".wl[117].w[0:1]"  0.99218820625318616 0.0078117937468137411;
	setAttr -s 2 ".wl[118].w[0:1]"  0.50381258982730759 0.49618741017269236;
	setAttr -s 2 ".wl[119].w[0:1]"  0.50070101858370508 0.49929898141629503;
	setAttr -s 2 ".wl[120].w[0:1]"  0.50072650871733382 0.49927349128266618;
	setAttr -s 2 ".wl[121].w[0:1]"  0.50407844367815524 0.49592155632184487;
	setAttr -s 2 ".wl[122].w[9:10]"  0.99663403207086632 0.0033659679291335809;
	setAttr -s 2 ".wl[123].w";
	setAttr ".wl[123].w[5]" 0.01051746064759986;
	setAttr ".wl[123].w[9]" 0.98948253935240016;
	setAttr -s 2 ".wl[124].w[5:6]"  0.010865531826438481 0.98913446817356154;
	setAttr -s 2 ".wl[125].w[6:7]"  0.99681560338581643 0.0031843966141835759;
	setAttr -s 2 ".wl[126].w";
	setAttr ".wl[126].w[5]" 0.0029848292513325993;
	setAttr ".wl[126].w[9]" 0.99701517074866741;
	setAttr -s 2 ".wl[127].w[3:4]"  0.50849706147726925 0.49150293852273069;
	setAttr -s 2 ".wl[128].w[3:4]"  0.50961520183696607 0.49038479816303399;
	setAttr -s 2 ".wl[129].w[5:6]"  0.0031692998022964662 0.99683070019770348;
	setAttr -s 2 ".wl[130].w[9:10]"  0.98085084960318181 0.01914915039681828;
	setAttr -s 2 ".wl[131].w[3:4]"  0.79310309072128382 0.20689690927871621;
	setAttr -s 2 ".wl[132].w[3:4]"  0.79520695180434797 0.20479304819565197;
	setAttr -s 2 ".wl[133].w[6:7]"  0.9822175063540689 0.017782493645931142;
	setAttr -s 2 ".wl[134].w[0:1]"  0.56180671747279298 0.43819328252720713;
	setAttr -s 2 ".wl[135].w[0:1]"  0.65980706366684683 0.34019293633315317;
	setAttr -s 2 ".wl[136].w[0:1]"  0.66424039101062282 0.33575960898937718;
	setAttr -s 2 ".wl[137].w[0:1]"  0.56545184473499988 0.43454815526500018;
	setAttr -s 2 ".wl[138].w[12:13]"  0.99923473443962652 0.0007652655603734886;
	setAttr -s 2 ".wl[139].w[12:13]"  0.99887093982881026 0.0011290601711897816;
	setAttr -s 2 ".wl[140].w[15:16]"  0.99839992745657291 0.0016000725434271823;
	setAttr -s 2 ".wl[141].w[15:16]"  0.99946725356427768 0.00053274643572238239;
	setAttr -s 2 ".wl[142].w[6:7]"  0.97292976871297188 0.027070231287028134;
	setAttr -s 2 ".wl[143].w[9:10]"  0.97091330308108481 0.029086696918915207;
	setAttr -s 2 ".wl[144].w[4:5]"  0.94894076364300883 0.051059236356991201;
	setAttr -s 2 ".wl[145].w[4:5]"  0.9553439091526934 0.044656090847306644;
	setAttr -s 2 ".wl[146].w[4:5]"  0.93383098310460999 0.066169016895390009;
	setAttr -s 2 ".wl[147].w[4:5]"  0.94550700779805508 0.054492992201944923;
	setAttr -s 2 ".wl[148].w[4:5]"  0.92938393367504091 0.070616066324959106;
	setAttr -s 2 ".wl[149].w[4:5]"  0.93853860174382853 0.061461398256171369;
	setAttr -s 2 ".wl[150].w[4:5]"  0.88004014908555406 0.11995985091444594;
	setAttr -s 2 ".wl[151].w[4:5]"  0.91456004674229241 0.085439953257707577;
	setAttr -s 2 ".wl[152].w[4:5]"  0.6408641363694314 0.3591358636305686;
	setAttr -s 2 ".wl[153].w[4:5]"  0.65101458292178804 0.34898541707821196;
	setAttr -s 2 ".wl[154].w[4:5]"  0.5 0.5;
	setAttr -s 2 ".wl[155].w[4:5]"  0.65290474403890186 0.34709525596109814;
	setAttr -s 2 ".wl[156].w[4:5]"  0.5 0.5;
	setAttr -s 2 ".wl[157].w[4:5]"  0.64295070570800772 0.35704929429199228;
	setAttr -s 2 ".wl[158].w[4:5]"  0.5 0.5;
	setAttr -s 2 ".wl[159].w[4:5]"  0.5 0.5;
	setAttr -s 2 ".wl[160].w[4:5]"  0.79200129817969089 0.20799870182030911;
	setAttr -s 2 ".wl[161].w[4:5]"  0.79896774243786739 0.20103225756213258;
	setAttr -s 2 ".wl[162].w[4:5]"  0.76029279902365543 0.23970720097634457;
	setAttr -s 2 ".wl[163].w[4:5]"  0.5042319862740422 0.4957680137259578;
	setAttr -s 2 ".wl[164].w[4:5]"  0.60592649119484865 0.39407350880515141;
	setAttr -s 2 ".wl[165].w[4:5]"  0.50406388294002269 0.49593611705997726;
	setAttr -s 2 ".wl[166].w[4:5]"  0.5118214221524271 0.4881785778475729;
	setAttr -s 2 ".wl[167].w[4:5]"  0.60324508405738886 0.39675491594261109;
	setAttr -s 2 ".wl[168].w[12:13]"  0.98063742853880087 0.019362571461199098;
	setAttr -s 2 ".wl[169].w[12:13]"  0.98514299935533933 0.014857000644660721;
	setAttr -s 2 ".wl[170].w[12:13]"  0.79512481995799789 0.20487518004200214;
	setAttr -s 2 ".wl[171].w[12:13]"  0.9918170377065868 0.0081829622934132748;
	setAttr -s 2 ".wl[172].w[12:13]"  0.90047376599235751 0.099526234007642558;
	setAttr -s 2 ".wl[173].w[12:13]"  0.99399252304146202 0.0060074769585379308;
	setAttr -s 2 ".wl[174].w[12:13]"  0.93900472200209195 0.06099527799790809;
	setAttr -s 2 ".wl[175].w[12:13]"  0.88540002671272189 0.11459997328727808;
	setAttr -s 2 ".wl[176].w[15:16]"  0.98301285021705687 0.016987149782943085;
	setAttr -s 2 ".wl[177].w[15:16]"  0.9826113104766776 0.017388689523322388;
	setAttr -s 2 ".wl[178].w[15:16]"  0.79461642450423708 0.20538357549576294;
	setAttr -s 2 ".wl[179].w[15:16]"  0.99540263195088441 0.0045973680491155746;
	setAttr -s 2 ".wl[180].w[15:16]"  0.9058195447671179 0.09418045523288214;
	setAttr -s 2 ".wl[181].w[15:16]"  0.98983323139300772 0.010166768606992177;
	setAttr -s 2 ".wl[182].w[15:16]"  0.93782584966816895 0.062174150331831032;
	setAttr -s 2 ".wl[183].w[15:16]"  0.87805627549158682 0.12194372450841313;
	setAttr -s 2 ".wl[184].w[12:13]"  0.59314434325477616 0.40685565674522384;
	setAttr -s 2 ".wl[185].w[12:13]"  0.6018475419026601 0.3981524580973399;
	setAttr -s 2 ".wl[186].w[12:13]"  0.55117681785272588 0.44882318214727418;
	setAttr -s 2 ".wl[187].w[12:13]"  0.65756313943364386 0.34243686056635619;
	setAttr -s 2 ".wl[188].w[12:13]"  0.56980556979105346 0.43019443020894654;
	setAttr -s 2 ".wl[189].w[12:13]"  0.67605104766203172 0.32394895233796839;
	setAttr -s 2 ".wl[190].w[12:13]"  0.59459698546215978 0.40540301453784033;
	setAttr -s 2 ".wl[191].w[12:13]"  0.56519774016275848 0.43480225983724163;
	setAttr -s 2 ".wl[192].w[15:16]"  0.59602742606842007 0.40397257393157987;
	setAttr -s 2 ".wl[193].w[15:16]"  0.59782240798064279 0.40217759201935727;
	setAttr -s 2 ".wl[194].w[15:16]"  0.5510344504173329 0.4489655495826671;
	setAttr -s 2 ".wl[195].w[15:16]"  0.71023744624987084 0.28976255375012916;
	setAttr -s 2 ".wl[196].w[15:16]"  0.57499135113070454 0.4250086488692954;
	setAttr -s 2 ".wl[197].w[15:16]"  0.63413732751963636 0.36586267248036364;
	setAttr -s 2 ".wl[198].w[15:16]"  0.59289776197985344 0.40710223802014656;
	setAttr -s 2 ".wl[199].w[15:16]"  0.55990968641663852 0.44009031358336148;
	setAttr -s 2 ".wl[200].w[12:13]"  0.51464167764811763 0.48535832235188237;
	setAttr -s 2 ".wl[201].w[12:13]"  0.515994236360103 0.484005763639897;
	setAttr -s 2 ".wl[202].w[13:14]"  0.99719363910336289 0.0028063608966371252;
	setAttr -s 2 ".wl[203].w[12:13]"  0.50732027487694464 0.49267972512305541;
	setAttr -s 2 ".wl[204].w[13:14]"  0.99958725110407576 0.0004127488959242862;
	setAttr -s 2 ".wl[205].w[12:13]"  0.51012707351031505 0.48987292648968506;
	setAttr -s 2 ".wl[206].w[13:14]"  0.99994548595898358 5.4514041016311699e-005;
	setAttr -s 2 ".wl[207].w[13:14]"  0.9994305525071937 0.00056944749280636445;
	setAttr -s 2 ".wl[208].w[15:16]"  0.51502961300283046 0.4849703869971696;
	setAttr -s 2 ".wl[209].w[15:16]"  0.51541678045413908 0.48458321954586092;
	setAttr -s 2 ".wl[210].w[16:17]"  0.99717671713914546 0.0028232828608544913;
	setAttr -s 2 ".wl[211].w[15:16]"  0.51300141061359583 0.48699858938640417;
	setAttr -s 2 ".wl[212].w[16:17]"  0.99958242263209862 0.00041757736790144404;
	setAttr -s 2 ".wl[213].w[15:16]"  0.50595439095949857 0.49404560904050143;
	setAttr -s 2 ".wl[214].w[16:17]"  0.99994314909267012 5.6850907329970515e-005;
	setAttr -s 2 ".wl[215].w[16:17]"  0.9994220667226289 0.0005779332773711189;
	setAttr -s 2 ".wl[216].w[13:14]"  0.98988359981141727 0.010116400188582739;
	setAttr -s 2 ".wl[217].w[13:14]"  0.99239603459937531 0.0076039654006247433;
	setAttr -s 2 ".wl[218].w[13:14]"  0.84654070719517061 0.15345929280482945;
	setAttr -s 2 ".wl[219].w[13:14]"  0.99891162886251017 0.0010883711374897896;
	setAttr -s 2 ".wl[220].w[13:14]"  0.97560499753680552 0.024395002463194506;
	setAttr -s 2 ".wl[221].w[13:14]"  0.99915055294171695 0.00084944705828304975;
	setAttr -s 2 ".wl[222].w[13:14]"  0.97711709619505882 0.022882903804941231;
	setAttr -s 2 ".wl[223].w[13:14]"  0.96654732939081323 0.033452670609186809;
	setAttr -s 2 ".wl[224].w[16:17]"  0.99118877371178393 0.0088112262882160779;
	setAttr -s 2 ".wl[225].w[16:17]"  0.99100576738280977 0.0089942326171903175;
	setAttr -s 2 ".wl[226].w[16:17]"  0.84507749580628011 0.15492250419371986;
	setAttr -s 2 ".wl[227].w[16:17]"  0.9994611852916867 0.00053881470831324148;
	setAttr -s 2 ".wl[228].w[16:17]"  0.98084630814772733 0.019153691852272673;
	setAttr -s 2 ".wl[229].w[16:17]"  0.99843126337398924 0.0015687366260107732;
	setAttr -s 2 ".wl[230].w[16:17]"  0.97612028149260166 0.023879718507398368;
	setAttr -s 2 ".wl[231].w[16:17]"  0.95830633159485656 0.041693668405143464;
	setAttr -s 2 ".wl[232].w[6:7]"  0.94178403263323096 0.058215967366769004;
	setAttr -s 2 ".wl[233].w[6:7]"  0.97964076242264841 0.02035923757735161;
	setAttr -s 2 ".wl[234].w[6:7]"  0.80621194239554583 0.19378805760445414;
	setAttr -s 2 ".wl[235].w[6:7]"  0.95146119023203446 0.048538809767965554;
	setAttr -s 2 ".wl[236].w[6:7]"  0.66667054147427007 0.33332945852572993;
	setAttr -s 2 ".wl[237].w[6:7]"  0.99690835335531058 0.003091646644689311;
	setAttr -s 2 ".wl[238].w[6:7]"  0.83353899042602764 0.16646100957397242;
	setAttr -s 2 ".wl[239].w[6:7]"  0.7994947524802124 0.20050524751978754;
	setAttr -s 2 ".wl[240].w[9:10]"  0.93526128686975174 0.064738713130248357;
	setAttr -s 2 ".wl[241].w[9:10]"  0.97719020302473425 0.022809796975265733;
	setAttr -s 2 ".wl[242].w[9:10]"  0.76666740877348938 0.23333259122651065;
	setAttr -s 2 ".wl[243].w[9:10]"  0.99662659915481699 0.0033734008451830697;
	setAttr -s 2 ".wl[244].w[9:10]"  0.76226382178194985 0.23773617821805015;
	setAttr -s 2 ".wl[245].w[9:10]"  0.94618875857424478 0.053811241425755267;
	setAttr -s 2 ".wl[246].w[9:10]"  0.79967531934246228 0.2003246806575377;
	setAttr -s 2 ".wl[247].w[9:10]"  0.6403799677601677 0.3596200322398323;
	setAttr -s 2 ".wl[248].w[6:7]"  0.50785593829831732 0.49214406170168268;
	setAttr -s 2 ".wl[249].w[7:8]"  0.9997049585084824 0.00029504149151764864;
	setAttr -s 2 ".wl[250].w[7:8]"  0.99985945066919324 0.00014054933080677383;
	setAttr -s 2 ".wl[251].w[6:7]"  0.51148461351992991 0.48851538648007009;
	setAttr -s 2 ".wl[252].w[6:7]"  0.5 0.5;
	setAttr -s 2 ".wl[253].w[6:7]"  0.50191316404926445 0.49808683595073561;
	setAttr -s 2 ".wl[254].w[6:7]"  0.5 0.5;
	setAttr -s 2 ".wl[255].w[7:8]"  0.99972702323625029 0.0002729767637497115;
	setAttr -s 2 ".wl[256].w[9:10]"  0.50112400138262925 0.49887599861737075;
	setAttr -s 2 ".wl[257].w[10:11]"  0.9997322730205408 0.00026772697945921065;
	setAttr -s 2 ".wl[258].w[10:11]"  0.99984692658959529 0.00015307341040467578;
	setAttr -s 2 ".wl[259].w[10:11]"  0.99994728257472076 5.2717425279249103e-005;
	setAttr -s 2 ".wl[260].w[10:11]"  0.9997679848607135 0.00023201513928646647;
	setAttr -s 2 ".wl[261].w[9:10]"  0.50255292048058242 0.49744707951941758;
	setAttr -s 2 ".wl[262].w[10:11]"  0.99989081358336096 0.0001091864166391033;
	setAttr -s 2 ".wl[263].w[9:10]"  0.5 0.5;
	setAttr -s 2 ".wl[264].w[6:7]"  0.5 0.5;
	setAttr -s 2 ".wl[265].w[7:8]"  0.99948674801150272 0.00051325198849736312;
	setAttr -s 2 ".wl[266].w[7:8]"  0.99977679912885953 0.00022320087114043404;
	setAttr -s 2 ".wl[267].w[6:7]"  0.5 0.5;
	setAttr -s 2 ".wl[268].w[7:8]"  0.99925740847357747 0.00074259152642254576;
	setAttr -s 2 ".wl[269].w[7:8]"  0.9998673557166684 0.00013264428333164576;
	setAttr -s 2 ".wl[270].w[7:8]"  0.99975156729323988 0.00024843270676018692;
	setAttr -s 2 ".wl[271].w[7:8]"  0.99916846476426269 0.00083153523573741169;
	setAttr -s 2 ".wl[272].w[10:11]"  0.99950907591050231 0.0004909240894977895;
	setAttr -s 2 ".wl[273].w[10:11]"  0.99953237044902166 0.00046762955097838526;
	setAttr -s 2 ".wl[274].w[10:11]"  0.99975892526871446 0.0002410747312856058;
	setAttr -s 2 ".wl[275].w[10:11]"  0.99988522066200614 0.00011477933799386669;
	setAttr -s 2 ".wl[276].w[10:11]"  0.9992725149539966 0.00072748504600340881;
	setAttr -s 2 ".wl[277].w[9:10]"  0.5 0.5;
	setAttr -s 2 ".wl[278].w[10:11]"  0.99972476514887021 0.00027523485112979464;
	setAttr -s 2 ".wl[279].w[10:11]"  0.99907940526645067 0.00092059473354933136;
	setAttr -s 2 ".wl[280].w[7:8]"  0.99541899678498003 0.0045810032150199282;
	setAttr -s 2 ".wl[281].w[7:8]"  0.99460781613563509 0.0053921838643649434;
	setAttr -s 2 ".wl[282].w[7:8]"  0.98399047390116867 0.016009526098831311;
	setAttr -s 2 ".wl[283].w[7:8]"  0.99340878401774491 0.0065912159822550985;
	setAttr -s 2 ".wl[284].w[7:8]"  0.88853520363409977 0.11146479636590027;
	setAttr -s 2 ".wl[285].w[7:8]"  0.99651016771948608 0.0034898322805139072;
	setAttr -s 2 ".wl[286].w[7:8]"  0.87184396498448857 0.12815603501551148;
	setAttr -s 2 ".wl[287].w[7:8]"  0.8450943137650303 0.1549056862349697;
	setAttr -s 2 ".wl[288].w[10:11]"  0.99419844281068559 0.0058015571893144892;
	setAttr -s 2 ".wl[289].w[10:11]"  0.99495339367367341 0.0050466063263265599;
	setAttr -s 2 ".wl[290].w[10:11]"  0.97148363310137609 0.028516366898623899;
	setAttr -s 2 ".wl[291].w[10:11]"  0.99660199712132513 0.0033980028786749134;
	setAttr -s 2 ".wl[292].w[10:11]"  0.82366830665370072 0.17633169334629931;
	setAttr -s 2 ".wl[293].w[10:11]"  0.99200888571180634 0.0079911142881936414;
	setAttr -s 2 ".wl[294].w[10:11]"  0.84207780101890839 0.15792219898109164;
	setAttr -s 2 ".wl[295].w[10:11]"  0.85997622276260544 0.14002377723739462;
	setAttr -s 2 ".wl[296].w[0:1]"  0.50072303215488401 0.49927696784511605;
	setAttr -s 2 ".wl[297].w[0:1]"  0.50063787210040078 0.49936212789959927;
	setAttr -s 2 ".wl[298].w[0:1]"  0.50076858352364761 0.49923141647635244;
	setAttr -s 2 ".wl[299].w";
	setAttr ".wl[299].w[5]" 0.0050871589647235324;
	setAttr ".wl[299].w[9]" 0.99491284103527655;
	setAttr -s 2 ".wl[300].w[3:4]"  0.78049280525384201 0.21950719474615799;
	setAttr -s 2 ".wl[301].w[5:6]"  0.0053298994516526015 0.99467010054834748;
	setAttr -s 2 ".wl[302].w";
	setAttr ".wl[302].w[5]" 0.013211030871968173;
	setAttr ".wl[302].w[9]" 0.98678896912803182;
	setAttr -s 2 ".wl[303].w[4:5]"  0.5 0.5;
	setAttr -s 2 ".wl[304].w[5:6]"  0.014055440467283617 0.98594455953271642;
	setAttr -s 2 ".wl[305].w";
	setAttr ".wl[305].w[5]" 0.021546318902399997;
	setAttr ".wl[305].w[9]" 0.9784536810976;
	setAttr -s 2 ".wl[306].w[2:3]"  0.5 0.5;
	setAttr -s 2 ".wl[307].w[5:6]"  0.021911544929850365 0.97808845507014963;
	setAttr -s 2 ".wl[308].w[0:1]"  0.60336983383677534 0.39663016616322466;
	setAttr -s 2 ".wl[309].w[0:1]"  0.67904530428712806 0.32095469571287194;
	setAttr -s 2 ".wl[310].w[0:1]"  0.60845349731377374 0.39154650268622626;
	setAttr -s 2 ".wl[311].w[13:14]"  0.95436518931165903 0.045634810688340965;
	setAttr -s 2 ".wl[312].w[12:13]"  0.99831464198651931 0.0016853580134807643;
	setAttr -s 2 ".wl[313].w[16:17]"  0.91850683474563843 0.081493165254361469;
	setAttr -s 2 ".wl[314].w[0:1]"  0.52811564177597015 0.47188435822402985;
	setAttr -s 2 ".wl[315].w[7:8]"  0.61933997321228518 0.38066002678771488;
	setAttr -s 2 ".wl[316].w[0:1]"  0.52617239114658299 0.47382760885341713;
	setAttr -s 2 ".wl[317].w[10:11]"  0.54036613669505751 0.45963386330494255;
	setAttr -s 2 ".wl[318].w[4:5]"  0.95538967013668397 0.044610329863316055;
	setAttr -s 2 ".wl[319].w[4:5]"  0.96633788566552192 0.03366211433447816;
	setAttr -s 2 ".wl[320].w[4:5]"  0.94208747108425328 0.05791252891574674;
	setAttr -s 2 ".wl[321].w[4:5]"  0.95782629178079515 0.042173708219204881;
	setAttr -s 2 ".wl[322].w[4:5]"  0.60749665219478288 0.39250334780521706;
	setAttr -s 2 ".wl[323].w[4:5]"  0.7077940302681891 0.29220596973181096;
	setAttr -s 2 ".wl[324].w[4:5]"  0.64455727395984708 0.35544272604015287;
	setAttr -s 2 ".wl[325].w[4:5]"  0.6868041843194469 0.31319581568055321;
	setAttr -s 2 ".wl[326].w[4:5]"  0.8706959993582597 0.12930400064174036;
	setAttr -s 2 ".wl[327].w[4:5]"  0.61148417357600204 0.38851582642399796;
	setAttr -s 2 ".wl[328].w[4:5]"  0.5 0.5;
	setAttr -s 2 ".wl[329].w[4:5]"  0.60681394501350616 0.39318605498649384;
	setAttr -s 2 ".wl[330].w[12:13]"  0.97786773093566404 0.022132269064335946;
	setAttr -s 2 ".wl[331].w[12:13]"  0.9951436770855081 0.0048563229144919636;
	setAttr -s 2 ".wl[332].w[12:13]"  0.9917674847950011 0.0082325152049989372;
	setAttr -s 2 ".wl[333].w[12:13]"  0.99412933842002293 0.0058706615799770743;
	setAttr -s 2 ".wl[334].w[15:16]"  0.97774318593987808 0.022256814060121841;
	setAttr -s 2 ".wl[335].w[15:16]"  0.99587320943466384 0.0041267905653361936;
	setAttr -s 2 ".wl[336].w[15:16]"  0.99166245057795943 0.0083375494220405324;
	setAttr -s 2 ".wl[337].w[15:16]"  0.99308949258861889 0.0069105074113811338;
	setAttr -s 2 ".wl[338].w[12:13]"  0.59359089955333255 0.40640910044666745;
	setAttr -s 2 ".wl[339].w[12:13]"  0.643080405203036 0.35691959479696395;
	setAttr -s 2 ".wl[340].w[12:13]"  0.71050723038081098 0.28949276961918902;
	setAttr -s 2 ".wl[341].w[12:13]"  0.63202332977666287 0.36797667022333719;
	setAttr -s 2 ".wl[342].w[15:16]"  0.59333804452120831 0.40666195547879169;
	setAttr -s 2 ".wl[343].w[15:16]"  0.65000278433467806 0.34999721566532199;
	setAttr -s 2 ".wl[344].w[15:16]"  0.70768796865835903 0.29231203134164097;
	setAttr -s 2 ".wl[345].w[15:16]"  0.62444738530381438 0.37555261469618562;
	setAttr -s 2 ".wl[346].w[12:13]"  0.51462682637825219 0.48537317362174787;
	setAttr -s 2 ".wl[347].w[12:13]"  0.51611067955420764 0.4838893204457923;
	setAttr -s 2 ".wl[348].w[12:13]"  0.50714613406512599 0.49285386593487407;
	setAttr -s 2 ".wl[349].w[12:13]"  0.5162689803961179 0.48373101960388221;
	setAttr -s 2 ".wl[350].w[15:16]"  0.51458563747241759 0.48541436252758241;
	setAttr -s 2 ".wl[351].w[15:16]"  0.51882143017999793 0.48117856982000212;
	setAttr -s 2 ".wl[352].w[15:16]"  0.50699144697888898 0.49300855302111113;
	setAttr -s 2 ".wl[353].w[15:16]"  0.51373615909569037 0.48626384090430974;
	setAttr -s 2 ".wl[354].w[13:14]"  0.9876178694800356 0.012382130519964459;
	setAttr -s 2 ".wl[355].w[13:14]"  0.99840358144494323 0.0015964185550567563;
	setAttr -s 2 ".wl[356].w[13:14]"  0.99923665607398571 0.00076334392601439351;
	setAttr -s 2 ".wl[357].w[13:14]"  0.99780880661256732 0.0021911933874326889;
	setAttr -s 2 ".wl[358].w[16:17]"  0.98752316580382626 0.012476834196173703;
	setAttr -s 2 ".wl[359].w[16:17]"  0.9985183341175411 0.001481665882458884;
	setAttr -s 2 ".wl[360].w[16:17]"  0.99921261383054094 0.00078738616945907237;
	setAttr -s 2 ".wl[361].w[16:17]"  0.99759436217558961 0.0024056378244104153;
	setAttr -s 2 ".wl[362].w[6:7]"  0.9852440131931155 0.01475598680688453;
	setAttr -s 2 ".wl[363].w[6:7]"  0.93003625700844661 0.069963742991553432;
	setAttr -s 2 ".wl[364].w[6:7]"  0.98938811071029154 0.010611889289708491;
	setAttr -s 2 ".wl[365].w[6:7]"  0.98760823069063253 0.012391769309367438;
	setAttr -s 2 ".wl[366].w[9:10]"  0.98304777316819769 0.016952226831802274;
	setAttr -s 2 ".wl[367].w[9:10]"  0.9862681234129953 0.013731876587004692;
	setAttr -s 2 ".wl[368].w[9:10]"  0.98807163663907882 0.01192836336092121;
	setAttr -s 2 ".wl[369].w[9:10]"  0.92290203607202581 0.077097963927974245;
	setAttr -s 2 ".wl[370].w[6:7]"  0.50640193813222756 0.49359806186777244;
	setAttr -s 2 ".wl[371].w[6:7]"  0.50942758741589755 0.49057241258410245;
	setAttr -s 2 ".wl[372].w[6:7]"  0.51730152669281138 0.48269847330718862;
	setAttr -s 2 ".wl[373].w[7:8]"  0.99980125579862267 0.00019874420137740869;
	setAttr -s 2 ".wl[374].w[9:10]"  0.5 0.5;
	setAttr -s 2 ".wl[375].w[10:11]"  0.99983306725003429 0.00016693274996567605;
	setAttr -s 2 ".wl[376].w[9:10]"  0.50180658500169328 0.49819341499830672;
	setAttr -s 2 ".wl[377].w[9:10]"  0.50219939073559372 0.49780060926440634;
	setAttr -s 2 ".wl[378].w[7:8]"  0.99984750331710215 0.00015249668289780681;
	setAttr -s 2 ".wl[379].w[6:7]"  0.5 0.5;
	setAttr -s 2 ".wl[380].w[7:8]"  0.99989183778397661 0.00010816221602346147;
	setAttr -s 2 ".wl[381].w[7:8]"  0.99959423772754419 0.00040576227245575652;
	setAttr -s 2 ".wl[382].w[10:11]"  0.99983624499698331 0.00016375500301664498;
	setAttr -s 2 ".wl[383].w[10:11]"  0.99964973614696462 0.00035026385303540432;
	setAttr -s 2 ".wl[384].w[10:11]"  0.99987981876065846 0.00012018123934157116;
	setAttr -s 2 ".wl[385].w[9:10]"  0.5 0.5;
	setAttr -s 2 ".wl[386].w[7:8]"  0.99899738885334466 0.0010026111466553654;
	setAttr -s 2 ".wl[387].w[7:8]"  0.99352843470470653 0.0064715652952935659;
	setAttr -s 2 ".wl[388].w[7:8]"  0.99634401522916072 0.0036559847708392715;
	setAttr -s 2 ".wl[389].w[7:8]"  0.99385434556360519 0.0061456544363948217;
	setAttr -s 2 ".wl[390].w[10:11]"  0.99881720988911527 0.0011827901108847187;
	setAttr -s 2 ".wl[391].w[10:11]"  0.99439037084317128 0.0056096291568287337;
	setAttr -s 2 ".wl[392].w[10:11]"  0.99578732609133591 0.004212673908664105;
	setAttr -s 2 ".wl[393].w[10:11]"  0.9919452031571212 0.0080547968428788686;
	setAttr -s 18 ".pm";
	setAttr ".pm[0]" -type "matrix" 3.2312236224862881e-016 -3.7697608929006663e-016 -1.0000000000000002 -0
		 0.97014250014533188 0.2425356250363333 3.7697608929006663e-016 -0 0.24253562503633358 -0.97014250014533188 3.2312236224862881e-016 -0
		 -6.2638287542727529 -0.52287497267355876 0.51371362083736838 1;
	setAttr ".pm[1]" -type "matrix" 2.2204460492503136e-016 -4.4408920985006281e-016 -1.0000000000000002 -0
		 1 -1.6653345369377333e-016 3.7697608929006663e-016 0 8.3266726846886679e-017 -1 3.2312236224862881e-016 -0
		 -7.0835687214962091 1.2319249950427973 0.51371362083736827 1;
	setAttr ".pm[2]" -type "matrix" 7.5497430631405477e-017 3.6826861918939362e-016 1.0000000000000002 -0
		 0.95202856128526514 0.30600918041311892 -3.3950077295980366e-016 0 -0.3060091804131187 0.95202856128526503 -2.0653248922114283e-016 -0
		 -7.5605505144955822 -3.7241769896787753 -0.51371362083736893 1;
	setAttr ".pm[3]" -type "matrix" 1.7232153367884199e-016 3.3410616118560003e-016 1.0000000000000002 -0
		 0.999314833766767 0.037011660509883915 -3.3950077295980356e-016 0 -0.037011660509883748 0.99931483376676689 -2.0653248922114278e-016 -0
		 -8.9090525426739031 -1.3646108628698217 -0.51371362083736882 1;
	setAttr ".pm[4]" -type "matrix" 1.7105953777740752e-016 3.3475404769320735e-016 1.0000000000000002 -0
		 0.99916805310057799 0.040782369514297902 -3.3950077295980361e-016 0 -0.040782369514297735 0.99916805310057788 -2.0653248922114281e-016 -0
		 -9.4982065208901219 -1.4004628675836461 -0.51371362083736904 1;
	setAttr ".pm[5]" -type "matrix" 1.0000000000000002 6.9111598229198405e-017 2.2109715158371595e-016 -0
		 -2.2847847049728785e-016 1.0000000000000002 5.2735593669694936e-016 0 -2.0653248922114288e-016 -4.2327252813834103e-016 1 -0
		 -0.51371362083737016 -10.625353082244315 -0.96794106753362685 1;
	setAttr ".pm[6]" -type "matrix" 0.9966128277891263 -0.054469631208034032 -0.061611125312095665 -0
		 0.054573307938201338 0.99850976663259672 2.8800746509904952e-015 -0 0.06151931035735219 -0.0033623229140787199 0.99810023005596871 -0
		 -1.4650603038272232 -8.7888080510333868 -1.1270770536915555 1;
	setAttr ".pm[7]" -type "matrix" 0.88499056427556555 -0.054469631208034032 -0.4624118947640482 -0
		 0.049835911099450465 0.99850976663259661 -0.022240231658087969 -0 0.46293421034538379 -0.0033623229140787173 0.88638626550534816 -0
		 -4.5005126883324671 -8.7888080510333886 0.77422326182627788 1;
	setAttr ".pm[8]" -type "matrix" 0.99851542345527888 -0.054469631208034039 -9.164052745176744e-005 -0
		 0.054469631208033789 0.99850976663259661 0.0033623229140789896 0 -9.1640527452196635e-005 -0.0033623229140787199 0.99999434317731783 -0
		 -6.7914874391701465 -8.7888080510333904 -2.6742947179723844 1;
	setAttr ".pm[9]" -type "matrix" 0.99661282778912619 -0.054469631208034247 -0.061611125312096081 -0
		 -0.054573307938201782 -0.99850976663259661 -3.4217420563642528e-015 0 -0.061519310357352329 0.0033623229140794424 -0.99810023005596848 -0
		 0.44111261247790995 8.8447677936568567 1.190373457429351 1;
	setAttr ".pm[10]" -type "matrix" 0.88499056427556522 -0.054469631208034247 -0.46241189476404898 -0
		 -0.04983591109945109 -0.99850976663259661 0.022240231658087681 0 -0.46293421034538429 0.0033623229140794355 -0.88638626550534783 -0
		 3.5912487902702916 8.8447749477281974 -0.29912798033914406 1;
	setAttr ".pm[11]" -type "matrix" 0.99851542345527877 -0.054469631208034247 -9.1640527452410033e-005 -0
		 -0.054469631208034192 -0.9985097666325965 -0.0033623229140795456 0 9.1640527451847969e-005 0.0033623229140794368 -0.99999434317731761 -0
		 5.7655858561917928 8.8447692639487201 2.6743924602587814 1;
	setAttr ".pm[12]" -type "matrix" -1.7120422554764184e-016 4.6605594732413664e-016 -1.0000000000000002 0
		 -0.99377881355723674 0.11137176358831155 3.7697608929006668e-016 0 0.11137176358831127 0.99377881355723674 3.2312236224862876e-016 -0
		 5.3429492326065606 -1.7055975504440521 -2.428655222465171e-015 1;
	setAttr ".pm[13]" -type "matrix" -2.9485172177649492e-016 -5.2194120619256652e-016 1.0000000000000002 -0
		 -0.98521175481967438 0.17134117475124841 -3.5599284716794041e-016 0 -0.17134117475124869 -0.98521175481967416 -4.4377600445085491e-016 0
		 3.1416918415361996 0.86033732793115825 3.1041866280206516e-015 1;
	setAttr ".pm[14]" -type "matrix" 1.0000000000000002 6.4809461839952587e-016 5.8614338024574425e-016 -0
		 -8.0008205701800303e-016 1 -8.326672684688695e-017 0 -4.4377600445085501e-016 3.8857805861880439e-016 0.99999999999999978 -0
		 3.5303682603389307e-015 0.087994642503059439 -0.8579477644047987 1;
	setAttr ".pm[15]" -type "matrix" 1.7120422554764196e-016 -4.6605594732413684e-016 -1.0000000000000002 -0
		 0.99377881355723707 -0.11137176358831143 3.7697608929006673e-016 0 -0.11137176358831118 -0.99377881355723718 3.2312236224862891e-016 0
		 -5.3429543836326392 1.7055950774497408 1.0274299999999976 1;
	setAttr ".pm[16]" -type "matrix" 2.9485172177649482e-016 5.2194120619256692e-016 1.0000000000000002 -0
		 0.9852117548196746 -0.17134117475124802 -3.5599284716794036e-016 0 0.17134117475124827 0.98521175481967471 -4.4377600445085501e-016 -0
		 -3.1416920760037859 -0.86034173348445975 -1.0274299999999974 1;
	setAttr ".pm[17]" -type "matrix" 1.0000000000000002 6.6447111079906277e-016 -3.5787107951555479e-016 -0
		 5.8769672376344274e-016 -1 1.3877787807814494e-016 0 -2.217313995258235e-016 -2.4980018054066037e-016 -1.0000000000000002 0
		 -1.0274299999999976 -0.087994599999997411 0.85794800000000115 1;
	setAttr ".gm" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0.49482263244127189 7.209991851303176 1.099376778977275 1;
	setAttr -s 18 ".ma";
	setAttr -s 18 ".dpf[0:17]"  4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4;
	setAttr -s 18 ".lw";
	setAttr -s 18 ".lw";
	setAttr ".mmi" yes;
	setAttr ".ucm" yes;
createNode objectSet -n "skinCluster1Set";
	setAttr ".ihi" 0;
	setAttr ".vo" yes;
createNode groupId -n "skinCluster1GroupId";
	setAttr ".ihi" 0;
createNode groupParts -n "skinCluster1GroupParts";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "vtx[*]";
createNode dagPose -n "bindPose1";
	setAttr -s 18 ".wm";
	setAttr -s 18 ".xm";
	setAttr ".xm[0]" -type "matrix" "xform" 1 1 1 0 0 0 0 0.51371362083737093 6.2036222964656247
		 1.0119383887851503 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 -0.43516214649359919 -0.55734541018930372 0.43516214649359919 0.55734541018930395 1
		 1 1 yes;
	setAttr ".xm[1]" -type "matrix" "xform" 1 1 1 0 0 0 0 0.90702801382143683 4.4408920985006262e-016
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0.12218326369570472 0.99250755668290302 1
		 1 1 yes;
	setAttr ".xm[2]" -type "matrix" "xform" 1 1 1 0 0 0 0 1.2539236556685847 2.2204460492503131e-016
		 2.4651903288156619e-032 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0.98793435037082922 0.15487323641406722 9.483250662403936e-018 6.0493531997467786e-017 1
		 1 1 yes;
	setAttr ".xm[3]" -type "matrix" "xform" 1 1 1 0 0 0 0 0.64700001929547479 4.4408920985006262e-016
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0.13656102782207494 0.99063165994237157 1
		 1 1 yes;
	setAttr ".xm[4]" -type "matrix" "xform" 1 1 1 0 0 0 0 0.59437107988959426 2.2204460492503131e-016
		 1.9694161627268558e-017 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 -0.0018867823774839313 0.99999822002454586 1
		 1 1 yes;
	setAttr ".xm[5]" -type "matrix" "xform" 1 1 1 0 0 0 0 1.0788319015179333 -1.1102230246251565e-014
		 3.5266289425573731e-017 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 -0.4896982822324637 -0.51009370940894216 -0.48969828223246364 0.51009370940894216 1
		 1 1 yes;
	setAttr ".xm[6]" -type "matrix" "xform" 1 1 1 0 0 0.054600433096169326 0 -0.10333414365516802
		 0.14785424248572809 0.3982206525521077 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 -0.50554899237080175 -0.52461965171819003 -0.47530451929045076 0.49323427626841343 1
		 1 1 yes;
	setAttr ".xm[7]" -type "matrix" "xform" 1 1 1 0 0 0 0 2.9602909174925496 0 3.9968028886505635e-015 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 -0.20833620339075731 0 0.97805727151160482 1
		 1 1 yes;
	setAttr ".xm[8]" -type "matrix" "xform" 1 1 1 0 0 0 0 2.7573611198725185 0 1.1102230246251565e-016 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0.23838115675863109 0 0.97117167591647613 1
		 1 1 yes;
	setAttr ".xm[9]" -type "matrix" "xform" 1 1 1 0 0 0.054600433096169326 0 -0.10333783552997033
		 0.14784954721997612 -0.39822062083737109 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0.52461965171818981 -0.50554899237080175 0.49323427626841321 0.47530451929045103 1
		 1 1 yes;
	setAttr ".xm[10]" -type "matrix" "xform" 1 1 1 0 0 0 0 -2.9602907115844057 -7.1540713406648138e-006
		 -4.9905011538786681e-006 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 -0.20833620339075745 0 0.97805727151160471 1
		 1 1 yes;
	setAttr ".xm[11]" -type "matrix" "xform" 1 1 1 0 0 0 0 -2.757363587933658 5.6837794755182358e-006
		 -2.3078811057231086e-006 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0.23838115675863117 0 0.97117167591647613 1
		 1 1 yes;
	setAttr ".xm[12]" -type "matrix" "xform" 1 1 1 0 0 0 0 -0.66159690419916739
		 -0.25610202743193788 0.51371362083737082 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 -0.98414822117468392 0.17734790316974516 1
		 1 1 yes;
	setAttr ".xm[13]" -type "matrix" "xform" 1 1 1 0 0 0 0 2.5678195165525359 -8.8817841970012523e-016
		 5.7017047007169253e-016 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0.98994949366116647 -0.14142135623731011 -8.6595605623549708e-018 6.0616923936484523e-017 1
		 1 1 yes;
	setAttr ".xm[14]" -type "matrix" "xform" 1 1 1 0 0 0 0 3.0813834198630432 -4.4408920985006262e-016
		 -4.6525910357850031e-016 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0.4551534975282383 -0.5411425816619978 0.45515349752823808 0.54114258166199802 1
		 1 1 yes;
	setAttr ".xm[15]" -type "matrix" "xform" 1 1 1 0 0 0 0 -0.66159294045226158
		 -0.25609791191484299 -0.51371637916262936 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 
		0 0 0.17734790316974505 0.98414822117468403 1 1 1 yes;
	setAttr ".xm[16]" -type "matrix" "xform" 1 1 1 0 0 0 0 -2.5678256760446541 -6.7679764019779043e-006
		 2.2204460492503131e-016 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 -0.98994949366116647 0.14142135623730981 -8.6595605623549523e-018 6.0616923936484523e-017 1
		 1 1 yes;
	setAttr ".xm[17]" -type "matrix" "xform" 1 1 1 0 0 0 0 -3.0813835720889537 -4.1807246624614436e-006
		 2.2204460492503131e-016 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0.45515349752823853 -0.54114258166199758 0.45515349752823808 0.54114258166199813 1
		 1 1 yes;
	setAttr -s 18 ".m";
	setAttr -s 18 ".p";
	setAttr ".bp" yes;
select -ne :time1;
	setAttr ".o" 31;
select -ne :renderPartition;
	setAttr -s 2 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 2 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :lightList1;
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
select -ne :ikSystem;
	setAttr -s 4 ".sol";
connectAttr "groupId2.id" "pCubeShape1.iog.og[1].gid";
connectAttr "tweakSet1.mwc" "pCubeShape1.iog.og[1].gco";
connectAttr "skinCluster1GroupId.id" "pCubeShape1.iog.og[2].gid";
connectAttr "skinCluster1Set.mwc" "pCubeShape1.iog.og[2].gco";
connectAttr "skinCluster1.og[0]" "pCubeShape1.i";
connectAttr "polyExtrudeFace11.out" "pCubeShape1Orig.i";
connectAttr "joint8_rotateX.o" "joint8.rx";
connectAttr "joint8_rotateY.o" "joint8.ry";
connectAttr "joint8_rotateZ.o" "joint8.rz";
connectAttr "joint8_translateX.o" "joint8.tx";
connectAttr "joint8_translateY.o" "joint8.ty";
connectAttr "joint8_translateZ.o" "joint8.tz";
connectAttr "joint8.s" "joint9.is";
connectAttr "joint9_rotateX.o" "joint9.rx";
connectAttr "joint9_rotateY.o" "joint9.ry";
connectAttr "joint9_rotateZ.o" "joint9.rz";
connectAttr "joint9.s" "joint10.is";
connectAttr "joint10_rotateX.o" "joint10.rx";
connectAttr "joint10_rotateY.o" "joint10.ry";
connectAttr "joint10_rotateZ.o" "joint10.rz";
connectAttr "joint10.s" "joint11.is";
connectAttr "joint11_rotateX.o" "joint11.rx";
connectAttr "joint11_rotateY.o" "joint11.ry";
connectAttr "joint11_rotateZ.o" "joint11.rz";
connectAttr "joint11.s" "joint12.is";
connectAttr "joint12_rotateX.o" "joint12.rx";
connectAttr "joint12_rotateY.o" "joint12.ry";
connectAttr "joint12_rotateZ.o" "joint12.rz";
connectAttr "joint12.s" "joint13.is";
connectAttr "joint11.s" "joint4.is";
connectAttr "joint4_translateX.o" "joint4.tx";
connectAttr "joint4_translateY.o" "joint4.ty";
connectAttr "joint4_translateZ.o" "joint4.tz";
connectAttr "joint4_rotateX.o" "joint4.rx";
connectAttr "joint4_rotateY.o" "joint4.ry";
connectAttr "joint4_rotateZ.o" "joint4.rz";
connectAttr "joint4.s" "joint5.is";
connectAttr "joint5_scaleX.o" "joint5.sx";
connectAttr "joint5_scaleY.o" "joint5.sy";
connectAttr "joint5_scaleZ.o" "joint5.sz";
connectAttr "joint5_rotateX.o" "joint5.rx";
connectAttr "joint5_rotateY.o" "joint5.ry";
connectAttr "joint5_rotateZ.o" "joint5.rz";
connectAttr "joint5_visibility.o" "joint5.v";
connectAttr "joint5_translateX.o" "joint5.tx";
connectAttr "joint5_translateY.o" "joint5.ty";
connectAttr "joint5_translateZ.o" "joint5.tz";
connectAttr "joint5.s" "joint6.is";
connectAttr "joint11.s" "joint17.is";
connectAttr "joint17_translateX.o" "joint17.tx";
connectAttr "joint17_translateY.o" "joint17.ty";
connectAttr "joint17_translateZ.o" "joint17.tz";
connectAttr "joint17_rotateX.o" "joint17.rx";
connectAttr "joint17_rotateY.o" "joint17.ry";
connectAttr "joint17_rotateZ.o" "joint17.rz";
connectAttr "joint17.s" "joint18.is";
connectAttr "joint18_scaleX.o" "joint18.sx";
connectAttr "joint18_scaleY.o" "joint18.sy";
connectAttr "joint18_scaleZ.o" "joint18.sz";
connectAttr "joint18_rotateX.o" "joint18.rx";
connectAttr "joint18_rotateY.o" "joint18.ry";
connectAttr "joint18_rotateZ.o" "joint18.rz";
connectAttr "joint18_visibility.o" "joint18.v";
connectAttr "joint18_translateX.o" "joint18.tx";
connectAttr "joint18_translateY.o" "joint18.ty";
connectAttr "joint18_translateZ.o" "joint18.tz";
connectAttr "joint18.s" "joint19.is";
connectAttr "joint8.s" "joint1.is";
connectAttr "joint1_translateX.o" "joint1.tx";
connectAttr "joint1_translateY.o" "joint1.ty";
connectAttr "joint1_translateZ.o" "joint1.tz";
connectAttr "joint1_rotateX.o" "joint1.rx";
connectAttr "joint1_rotateY.o" "joint1.ry";
connectAttr "joint1_rotateZ.o" "joint1.rz";
connectAttr "joint1.s" "joint2.is";
connectAttr "joint2_rotateX.o" "joint2.rx";
connectAttr "joint2_rotateY.o" "joint2.ry";
connectAttr "joint2_rotateZ.o" "joint2.rz";
connectAttr "joint2.s" "joint3.is";
connectAttr "joint8.s" "joint14.is";
connectAttr "joint14_rotateX.o" "joint14.rx";
connectAttr "joint14_rotateY.o" "joint14.ry";
connectAttr "joint14_rotateZ.o" "joint14.rz";
connectAttr "joint14.s" "joint15.is";
connectAttr "joint15_scaleX.o" "joint15.sx";
connectAttr "joint15_scaleY.o" "joint15.sy";
connectAttr "joint15_scaleZ.o" "joint15.sz";
connectAttr "joint15_rotateX.o" "joint15.rx";
connectAttr "joint15_rotateY.o" "joint15.ry";
connectAttr "joint15_rotateZ.o" "joint15.rz";
connectAttr "joint15_visibility.o" "joint15.v";
connectAttr "joint15_translateX.o" "joint15.tx";
connectAttr "joint15_translateY.o" "joint15.ty";
connectAttr "joint15_translateZ.o" "joint15.tz";
connectAttr "joint15.s" "joint16.is";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[0].llnk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.lnk[0].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[1].llnk";
connectAttr ":initialParticleSE.msg" "lightLinker1.lnk[1].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[0].sllk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.slnk[0].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[1].sllk";
connectAttr ":initialParticleSE.msg" "lightLinker1.slnk[1].solk";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "polyTweak1.out" "polyExtrudeFace1.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace1.mp";
connectAttr "polyCube1.out" "polyTweak1.ip";
connectAttr "polyExtrudeFace1.out" "polyExtrudeFace2.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace2.mp";
connectAttr "polyExtrudeFace2.out" "polyExtrudeFace3.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace3.mp";
connectAttr "polyTweak2.out" "polyExtrudeFace4.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace4.mp";
connectAttr "polyExtrudeFace3.out" "polyTweak2.ip";
connectAttr "polyExtrudeFace4.out" "polyExtrudeFace5.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace5.mp";
connectAttr "polyExtrudeFace5.out" "polyExtrudeFace6.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace6.mp";
connectAttr "polyExtrudeFace6.out" "polyExtrudeFace7.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace7.mp";
connectAttr "polyTweak3.out" "polyExtrudeFace8.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace8.mp";
connectAttr "polyExtrudeFace7.out" "polyTweak3.ip";
connectAttr "polyExtrudeFace8.out" "polyExtrudeFace9.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace9.mp";
connectAttr "polyExtrudeFace9.out" "polyExtrudeFace10.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace10.mp";
connectAttr "polyExtrudeFace10.out" "polyExtrudeFace11.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace11.mp";
connectAttr "groupParts2.og" "tweak1.ip[0].ig";
connectAttr "groupId2.id" "tweak1.ip[0].gi";
connectAttr "groupId2.msg" "tweakSet1.gn" -na;
connectAttr "pCubeShape1.iog.og[1]" "tweakSet1.dsm" -na;
connectAttr "tweak1.msg" "tweakSet1.ub[0]";
connectAttr "pCubeShape1Orig.w" "groupParts2.ig";
connectAttr "groupId2.id" "groupParts2.gi";
connectAttr "tweak1.og[0]" "polySmoothFace1.ip";
connectAttr "polyTweak4.out" "polyTriangulate1.ip";
connectAttr "polySmoothFace1.out" "polyTweak4.ip";
connectAttr "skinCluster1GroupParts.og" "skinCluster1.ip[0].ig";
connectAttr "skinCluster1GroupId.id" "skinCluster1.ip[0].gi";
connectAttr "bindPose1.msg" "skinCluster1.bp";
connectAttr "joint8.wm" "skinCluster1.ma[0]";
connectAttr "joint9.wm" "skinCluster1.ma[1]";
connectAttr "joint10.wm" "skinCluster1.ma[2]";
connectAttr "joint11.wm" "skinCluster1.ma[3]";
connectAttr "joint12.wm" "skinCluster1.ma[4]";
connectAttr "joint13.wm" "skinCluster1.ma[5]";
connectAttr "joint4.wm" "skinCluster1.ma[6]";
connectAttr "joint5.wm" "skinCluster1.ma[7]";
connectAttr "joint6.wm" "skinCluster1.ma[8]";
connectAttr "joint17.wm" "skinCluster1.ma[9]";
connectAttr "joint18.wm" "skinCluster1.ma[10]";
connectAttr "joint19.wm" "skinCluster1.ma[11]";
connectAttr "joint1.wm" "skinCluster1.ma[12]";
connectAttr "joint2.wm" "skinCluster1.ma[13]";
connectAttr "joint3.wm" "skinCluster1.ma[14]";
connectAttr "joint14.wm" "skinCluster1.ma[15]";
connectAttr "joint15.wm" "skinCluster1.ma[16]";
connectAttr "joint16.wm" "skinCluster1.ma[17]";
connectAttr "joint8.liw" "skinCluster1.lw[0]";
connectAttr "joint9.liw" "skinCluster1.lw[1]";
connectAttr "joint10.liw" "skinCluster1.lw[2]";
connectAttr "joint11.liw" "skinCluster1.lw[3]";
connectAttr "joint12.liw" "skinCluster1.lw[4]";
connectAttr "joint13.liw" "skinCluster1.lw[5]";
connectAttr "joint4.liw" "skinCluster1.lw[6]";
connectAttr "joint5.liw" "skinCluster1.lw[7]";
connectAttr "joint6.liw" "skinCluster1.lw[8]";
connectAttr "joint17.liw" "skinCluster1.lw[9]";
connectAttr "joint18.liw" "skinCluster1.lw[10]";
connectAttr "joint19.liw" "skinCluster1.lw[11]";
connectAttr "joint1.liw" "skinCluster1.lw[12]";
connectAttr "joint2.liw" "skinCluster1.lw[13]";
connectAttr "joint3.liw" "skinCluster1.lw[14]";
connectAttr "joint14.liw" "skinCluster1.lw[15]";
connectAttr "joint15.liw" "skinCluster1.lw[16]";
connectAttr "joint16.liw" "skinCluster1.lw[17]";
connectAttr "skinCluster1GroupId.msg" "skinCluster1Set.gn" -na;
connectAttr "pCubeShape1.iog.og[2]" "skinCluster1Set.dsm" -na;
connectAttr "skinCluster1.msg" "skinCluster1Set.ub[0]";
connectAttr "polyTriangulate1.out" "skinCluster1GroupParts.ig";
connectAttr "skinCluster1GroupId.id" "skinCluster1GroupParts.gi";
connectAttr "joint8.msg" "bindPose1.m[0]";
connectAttr "joint9.msg" "bindPose1.m[1]";
connectAttr "joint10.msg" "bindPose1.m[2]";
connectAttr "joint11.msg" "bindPose1.m[3]";
connectAttr "joint12.msg" "bindPose1.m[4]";
connectAttr "joint13.msg" "bindPose1.m[5]";
connectAttr "joint4.msg" "bindPose1.m[6]";
connectAttr "joint5.msg" "bindPose1.m[7]";
connectAttr "joint6.msg" "bindPose1.m[8]";
connectAttr "joint17.msg" "bindPose1.m[9]";
connectAttr "joint18.msg" "bindPose1.m[10]";
connectAttr "joint19.msg" "bindPose1.m[11]";
connectAttr "joint1.msg" "bindPose1.m[12]";
connectAttr "joint2.msg" "bindPose1.m[13]";
connectAttr "joint3.msg" "bindPose1.m[14]";
connectAttr "joint14.msg" "bindPose1.m[15]";
connectAttr "joint15.msg" "bindPose1.m[16]";
connectAttr "joint16.msg" "bindPose1.m[17]";
connectAttr "bindPose1.w" "bindPose1.p[0]";
connectAttr "bindPose1.m[0]" "bindPose1.p[1]";
connectAttr "bindPose1.m[1]" "bindPose1.p[2]";
connectAttr "bindPose1.m[2]" "bindPose1.p[3]";
connectAttr "bindPose1.m[3]" "bindPose1.p[4]";
connectAttr "bindPose1.m[4]" "bindPose1.p[5]";
connectAttr "bindPose1.m[3]" "bindPose1.p[6]";
connectAttr "bindPose1.m[6]" "bindPose1.p[7]";
connectAttr "bindPose1.m[7]" "bindPose1.p[8]";
connectAttr "bindPose1.m[3]" "bindPose1.p[9]";
connectAttr "bindPose1.m[9]" "bindPose1.p[10]";
connectAttr "bindPose1.m[10]" "bindPose1.p[11]";
connectAttr "bindPose1.m[0]" "bindPose1.p[12]";
connectAttr "bindPose1.m[12]" "bindPose1.p[13]";
connectAttr "bindPose1.m[13]" "bindPose1.p[14]";
connectAttr "bindPose1.m[0]" "bindPose1.p[15]";
connectAttr "bindPose1.m[15]" "bindPose1.p[16]";
connectAttr "bindPose1.m[16]" "bindPose1.p[17]";
connectAttr "joint8.bps" "bindPose1.wm[0]";
connectAttr "joint9.bps" "bindPose1.wm[1]";
connectAttr "joint10.bps" "bindPose1.wm[2]";
connectAttr "joint11.bps" "bindPose1.wm[3]";
connectAttr "joint12.bps" "bindPose1.wm[4]";
connectAttr "joint13.bps" "bindPose1.wm[5]";
connectAttr "joint4.bps" "bindPose1.wm[6]";
connectAttr "joint5.bps" "bindPose1.wm[7]";
connectAttr "joint6.bps" "bindPose1.wm[8]";
connectAttr "joint17.bps" "bindPose1.wm[9]";
connectAttr "joint18.bps" "bindPose1.wm[10]";
connectAttr "joint19.bps" "bindPose1.wm[11]";
connectAttr "joint1.bps" "bindPose1.wm[12]";
connectAttr "joint2.bps" "bindPose1.wm[13]";
connectAttr "joint3.bps" "bindPose1.wm[14]";
connectAttr "joint14.bps" "bindPose1.wm[15]";
connectAttr "joint15.bps" "bindPose1.wm[16]";
connectAttr "joint16.bps" "bindPose1.wm[17]";
connectAttr "lightLinker1.msg" ":lightList1.ln" -na;
connectAttr "pCubeShape1.iog" ":initialShadingGroup.dsm" -na;
// End of anim-test.ma
