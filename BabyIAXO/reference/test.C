void test() {
   gSystem->Load("libGeom");
   new TGeoManager("Default", "Geometry imported from GDML");

   Double_t dx,dy,dz;
   Double_t dx1, dx2, dy1, dy2;
   Double_t vert[20], par[20];
   Double_t theta, phi, h1, bl1, tl1, alpha1, h2, bl2, tl2, alpha2;
   Double_t twist;
   Double_t origin[3];
   Double_t rmin, rmax, rmin1, rmax1, rmin2, rmax2;
   Double_t r, rlo, rhi;
   Double_t phi1, phi2;
   Double_t a,b;
   Double_t point[3], norm[3];
   Double_t rin, stin, rout, stout;
   Double_t thx, phx, thy, phy, thz, phz;
   Double_t alpha, theta1, theta2, phi1, phi2, dphi;
   Double_t tr[3], rot[9];
   Double_t z, density, radl, absl, w;
   Double_t lx,ly,lz,tx,ty,tz;
   Double_t xvert[50], yvert[50];
   Double_t zsect,x0,y0,scale0;
   Int_t nel, numed, nz, nedges, nvert;
   TGeoBoolNode *pBoolNode = 0;

   // MATERIALS, MIXTURES AND TRACKING MEDIA
// Mixture: Vacuum
   nel     = 2;
   density = 9.9999999999999995e-21;
   pMat17 = new TGeoMixture("Vacuum", nel,density);
      a = 14.006740000000001;   z = 7;   w = 0.69999999999999996;  // N
   pMat17->DefineElement(0,a,z,w);
      a = 15.9994;   z = 8;   w = 0.29999999999999999;  // O
   pMat17->DefineElement(1,a,z,w);
   pMat17->SetIndex(16);
// Medium: Vacuum
   numed   = 18;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed17 = new TGeoMedium("Vacuum", numed,pMat17, par);
// Mixture: Ar_iC4H10
   nel     = 2;
   density = 2.3507195599999999;
   pMat9 = new TGeoMixture("Ar_iC4H10", nel,density);
      a = 12.0107;   z = 6;   w = 0.016531652277443042;  // C
   pMat9->DefineElement(0,a,z,w);
      a = 1.0079400000000001;   z = 1;   w = 0.0034683477225569575;  // H
   pMat9->DefineElement(1,a,z,w);
   pMat9->SetIndex(8);
// Medium: Ar_iC4H10
   numed   = 10;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed9 = new TGeoMedium("Ar_iC4H10", numed,pMat9, par);
// Mixture: Kapton
   nel     = 4;
   density = 1.4199999999999999;
   pMat20 = new TGeoMixture("Kapton", nel,density);
      a = 12.0107;   z = 6;   w = 0.691133;  // C
   pMat20->DefineElement(0,a,z,w);
      a = 1.0079400000000001;   z = 1;   w = 0.026362;  // H
   pMat20->DefineElement(1,a,z,w);
      a = 14.006740000000001;   z = 7;   w = 0.073270000000000002;  // N
   pMat20->DefineElement(2,a,z,w);
      a = 15.9994;   z = 8;   w = 0.209235;  // O
   pMat20->DefineElement(3,a,z,w);
   pMat20->SetIndex(19);
// Medium: Kapton
   numed   = 21;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed20 = new TGeoMedium("Kapton", numed,pMat20, par);
// Mixture: Copper
   nel     = 1;
   density = 8.9600000000000009;
   pMat13 = new TGeoMixture("Copper", nel,density);
      a = 63.545999999999999;   z = 29;   w = 1;  // CU
   pMat13->DefineElement(0,a,z,w);
   pMat13->SetIndex(12);
// Medium: Copper
   numed   = 14;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed13 = new TGeoMedium("Copper", numed,pMat13, par);
// Mixture: PTFE
   nel     = 2;
   density = 2.2000000000000002;
   pMat21 = new TGeoMixture("PTFE", nel,density);
      a = 12.0107;   z = 6;   w = 0.24018300000000001;  // C
   pMat21->DefineElement(0,a,z,w);
      a = 18.998403199999998;   z = 9;   w = 0.75981699999999996;  // F
   pMat21->DefineElement(1,a,z,w);
   pMat21->SetIndex(20);
// Medium: PTFE
   numed   = 22;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed21 = new TGeoMedium("PTFE", numed,pMat21, par);
// Mixture: Mylar
   nel     = 3;
   density = 1.3999999999999999;
   pMat22 = new TGeoMixture("Mylar", nel,density);
      a = 12.0107;   z = 6;   w = 0.62501700000000004;  // C
   pMat22->DefineElement(0,a,z,w);
      a = 1.0079400000000001;   z = 1;   w = 0.041959000000000003;  // H
   pMat22->DefineElement(1,a,z,w);
      a = 15.9994;   z = 8;   w = 0.33302500000000002;  // O
   pMat22->DefineElement(2,a,z,w);
   pMat22->SetIndex(21);
// Medium: Mylar
   numed   = 23;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed22 = new TGeoMedium("Mylar", numed,pMat22, par);
// Mixture: Lead
   nel     = 1;
   density = 11.34;
   pMat19 = new TGeoMixture("Lead", nel,density);
      a = 207.19999999999999;   z = 82;   w = 1;  // PB
   pMat19->DefineElement(0,a,z,w);
   pMat19->SetIndex(18);
// Medium: Lead
   numed   = 20;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed19 = new TGeoMedium("Lead", numed,pMat19, par);
// Mixture: BC408
   nel     = 2;
   density = 1.03;
   pMat23 = new TGeoMixture("BC408", nel,density);
      a = 12.0107;   z = 6;   w = 0.91500000000000004;  // C
   pMat23->DefineElement(0,a,z,w);
      a = 1.0079400000000001;   z = 1;   w = 0.085000000000000006;  // H
   pMat23->DefineElement(1,a,z,w);
   pMat23->SetIndex(22);
// Medium: BC408
   numed   = 24;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   pMed23 = new TGeoMedium("BC408", numed,pMat23, par);

   // TRANSFORMATION MATRICES
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 0;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix113 = new TGeoHMatrix("");
   pMatrix113->SetTranslation(tr);
   pMatrix113->SetRotation(rot);
   pMatrix113->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = -1.5250000000000001;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix114 = new TGeoHMatrix("");
   pMatrix114->SetTranslation(tr);
   pMatrix114->SetRotation(rot);
   pMatrix114->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = -1.5600000000000001;
   rot[0] =0.70710678118654757;    rot[1] = 0.70710678118654746;    rot[2] = 0;
   rot[3] =-0.70710678118654746;    rot[4] = 0.70710678118654757;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix115 = new TGeoHMatrix("");
   pMatrix115->SetTranslation(tr);
   pMatrix115->SetRotation(rot);
   pMatrix115->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix115->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = -2.1000000000000001;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix116 = new TGeoHMatrix("");
   pMatrix116->SetTranslation(tr);
   pMatrix116->SetRotation(rot);
   pMatrix116->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 0.75;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix117 = new TGeoHMatrix("");
   pMatrix117->SetTranslation(tr);
   pMatrix117->SetRotation(rot);
   pMatrix117->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 0.75;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix118 = new TGeoHMatrix("");
   pMatrix118->SetTranslation(tr);
   pMatrix118->SetRotation(rot);
   pMatrix118->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 1.4998;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix119 = new TGeoHMatrix("");
   pMatrix119->SetTranslation(tr);
   pMatrix119->SetRotation(rot);
   pMatrix119->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 2.25;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix120 = new TGeoHMatrix("");
   pMatrix120->SetTranslation(tr);
   pMatrix120->SetRotation(rot);
   pMatrix120->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 3.125;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix121 = new TGeoHMatrix("");
   pMatrix121->SetTranslation(tr);
   pMatrix121->SetRotation(rot);
   pMatrix121->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 2.5250000000000004;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix122 = new TGeoHMatrix("");
   pMatrix122->SetTranslation(tr);
   pMatrix122->SetRotation(rot);
   pMatrix122->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 2.5250000000000004;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix123 = new TGeoHMatrix("");
   pMatrix123->SetTranslation(tr);
   pMatrix123->SetRotation(rot);
   pMatrix123->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 1.6500000000000001;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix124 = new TGeoHMatrix("");
   pMatrix124->SetTranslation(tr);
   pMatrix124->SetRotation(rot);
   pMatrix124->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 1.6500000000000001;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix125 = new TGeoHMatrix("");
   pMatrix125->SetTranslation(tr);
   pMatrix125->SetRotation(rot);
   pMatrix125->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 4.4450000000000003;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix126 = new TGeoHMatrix("");
   pMatrix126->SetTranslation(tr);
   pMatrix126->SetRotation(rot);
   pMatrix126->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 4.1000000000000005;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix127 = new TGeoHMatrix("");
   pMatrix127->SetTranslation(tr);
   pMatrix127->SetRotation(rot);
   pMatrix127->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 16.374750000000002;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix128 = new TGeoHMatrix("");
   pMatrix128->SetTranslation(tr);
   pMatrix128->SetRotation(rot);
   pMatrix128->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 0;    tr[2] = 3.3000000000000003;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix129 = new TGeoHMatrix("");
   pMatrix129->SetTranslation(tr);
   pMatrix129->SetRotation(rot);
   pMatrix129->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 0;    tr[1] = 0;    tr[2] = 38.800000000000004;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix130 = new TGeoHMatrix("");
   pMatrix130->SetTranslation(tr);
   pMatrix130->SetRotation(rot);
   pMatrix130->SetBit(TGeoMatrix::kGeoTranslation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = -20;    tr[2] = 33.300000000000004;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-1;    rot[4] = 6.123233995736766e-17;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix131 = new TGeoHMatrix("");
   pMatrix131->SetTranslation(tr);
   pMatrix131->SetRotation(rot);
   pMatrix131->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix131->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 0;    tr[2] = 33.300000000000004;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-1;    rot[4] = 6.123233995736766e-17;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix132 = new TGeoHMatrix("");
   pMatrix132->SetTranslation(tr);
   pMatrix132->SetRotation(rot);
   pMatrix132->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix132->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 20;    tr[2] = 33.300000000000004;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-1;    rot[4] = 6.123233995736766e-17;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix133 = new TGeoHMatrix("");
   pMatrix133->SetTranslation(tr);
   pMatrix133->SetRotation(rot);
   pMatrix133->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix133->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = -18.930000000000003;    tr[1] = 5;    tr[2] = -26.700000000000003;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix134 = new TGeoHMatrix("");
   pMatrix134->SetTranslation(tr);
   pMatrix134->SetRotation(rot);
   pMatrix134->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix134->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 5;    tr[2] = -26.700000000000003;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix135 = new TGeoHMatrix("");
   pMatrix135->SetTranslation(tr);
   pMatrix135->SetRotation(rot);
   pMatrix135->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix135->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 21.07;    tr[1] = 5;    tr[2] = -26.700000000000003;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 1;    rot[5] = 0;
   rot[6] =0;    rot[7] = 0;    rot[8] = 1;
   TGeoHMatrix *pMatrix136 = new TGeoHMatrix("");
   pMatrix136->SetTranslation(tr);
   pMatrix136->SetRotation(rot);
   pMatrix136->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix136->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 30.000050000000002;    tr[2] = -14.200000000000001;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-6.123233995736766e-17;    rot[4] = 3.749399456654644e-33;    rot[5] = 1;
   rot[6] =1;    rot[7] = -6.123233995736766e-17;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix137 = new TGeoHMatrix("");
   pMatrix137->SetTranslation(tr);
   pMatrix137->SetRotation(rot);
   pMatrix137->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix137->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 30.000050000000002;    tr[2] = 5.8000000000000007;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-6.123233995736766e-17;    rot[4] = 3.749399456654644e-33;    rot[5] = 1;
   rot[6] =1;    rot[7] = -6.123233995736766e-17;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix138 = new TGeoHMatrix("");
   pMatrix138->SetTranslation(tr);
   pMatrix138->SetRotation(rot);
   pMatrix138->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix138->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 30.000050000000002;    tr[2] = 25.800000000000001;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-6.123233995736766e-17;    rot[4] = 3.749399456654644e-33;    rot[5] = 1;
   rot[6] =1;    rot[7] = -6.123233995736766e-17;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix139 = new TGeoHMatrix("");
   pMatrix139->SetTranslation(tr);
   pMatrix139->SetRotation(rot);
   pMatrix139->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix139->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 32.500099999999996;    tr[2] = -14.200000000000001;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-6.123233995736766e-17;    rot[4] = 3.749399456654644e-33;    rot[5] = 1;
   rot[6] =1;    rot[7] = -6.123233995736766e-17;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix140 = new TGeoHMatrix("");
   pMatrix140->SetTranslation(tr);
   pMatrix140->SetRotation(rot);
   pMatrix140->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix140->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 32.500099999999996;    tr[2] = 5.8000000000000007;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-6.123233995736766e-17;    rot[4] = 3.749399456654644e-33;    rot[5] = 1;
   rot[6] =1;    rot[7] = -6.123233995736766e-17;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix141 = new TGeoHMatrix("");
   pMatrix141->SetTranslation(tr);
   pMatrix141->SetRotation(rot);
   pMatrix141->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix141->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = 32.500099999999996;    tr[2] = 25.800000000000001;
   rot[0] =6.123233995736766e-17;    rot[1] = 1;    rot[2] = 0;
   rot[3] =-6.123233995736766e-17;    rot[4] = 3.749399456654644e-33;    rot[5] = 1;
   rot[6] =1;    rot[7] = -6.123233995736766e-17;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix142 = new TGeoHMatrix("");
   pMatrix142->SetTranslation(tr);
   pMatrix142->SetRotation(rot);
   pMatrix142->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix142->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = -18.930000000000003;    tr[1] = -34.5;    tr[2] = 0.80000000000000004;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 6.123233995736766e-17;    rot[5] = -1;
   rot[6] =0;    rot[7] = 1;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix143 = new TGeoHMatrix("");
   pMatrix143->SetTranslation(tr);
   pMatrix143->SetRotation(rot);
   pMatrix143->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix143->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 1.0700000000000001;    tr[1] = -34.5;    tr[2] = 0.80000000000000004;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 6.123233995736766e-17;    rot[5] = -1;
   rot[6] =0;    rot[7] = 1;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix144 = new TGeoHMatrix("");
   pMatrix144->SetTranslation(tr);
   pMatrix144->SetRotation(rot);
   pMatrix144->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix144->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 21.07;    tr[1] = -34.5;    tr[2] = 0.80000000000000004;
   rot[0] =1;    rot[1] = 0;    rot[2] = 0;
   rot[3] =0;    rot[4] = 6.123233995736766e-17;    rot[5] = -1;
   rot[6] =0;    rot[7] = 1;    rot[8] = 6.123233995736766e-17;
   TGeoHMatrix *pMatrix145 = new TGeoHMatrix("");
   pMatrix145->SetTranslation(tr);
   pMatrix145->SetRotation(rot);
   pMatrix145->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix145->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = -31.430000000000003;    tr[1] = -20;    tr[2] = -4.2000000000000002;
   rot[0] =6.123233995736766e-17;    rot[1] = 0;    rot[2] = -1;
   rot[3] =1;    rot[4] = 6.123233995736766e-17;    rot[5] = 6.123233995736766e-17;
   rot[6] =6.123233995736766e-17;    rot[7] = -1;    rot[8] = 3.749399456654644e-33;
   TGeoHMatrix *pMatrix146 = new TGeoHMatrix("");
   pMatrix146->SetTranslation(tr);
   pMatrix146->SetRotation(rot);
   pMatrix146->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix146->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = -31.430000000000003;    tr[1] = 0;    tr[2] = -4.2000000000000002;
   rot[0] =6.123233995736766e-17;    rot[1] = 0;    rot[2] = -1;
   rot[3] =1;    rot[4] = 6.123233995736766e-17;    rot[5] = 6.123233995736766e-17;
   rot[6] =6.123233995736766e-17;    rot[7] = -1;    rot[8] = 3.749399456654644e-33;
   TGeoHMatrix *pMatrix147 = new TGeoHMatrix("");
   pMatrix147->SetTranslation(tr);
   pMatrix147->SetRotation(rot);
   pMatrix147->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix147->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = -31.430000000000003;    tr[1] = 20;    tr[2] = -4.2000000000000002;
   rot[0] =6.123233995736766e-17;    rot[1] = 0;    rot[2] = -1;
   rot[3] =1;    rot[4] = 6.123233995736766e-17;    rot[5] = 6.123233995736766e-17;
   rot[6] =6.123233995736766e-17;    rot[7] = -1;    rot[8] = 3.749399456654644e-33;
   TGeoHMatrix *pMatrix148 = new TGeoHMatrix("");
   pMatrix148->SetTranslation(tr);
   pMatrix148->SetRotation(rot);
   pMatrix148->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix148->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 33.57;    tr[1] = -20;    tr[2] = -4.2000000000000002;
   rot[0] =6.123233995736766e-17;    rot[1] = 0;    rot[2] = -1;
   rot[3] =1;    rot[4] = 6.123233995736766e-17;    rot[5] = 6.123233995736766e-17;
   rot[6] =6.123233995736766e-17;    rot[7] = -1;    rot[8] = 3.749399456654644e-33;
   TGeoHMatrix *pMatrix149 = new TGeoHMatrix("");
   pMatrix149->SetTranslation(tr);
   pMatrix149->SetRotation(rot);
   pMatrix149->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix149->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 33.57;    tr[1] = 0;    tr[2] = -4.2000000000000002;
   rot[0] =6.123233995736766e-17;    rot[1] = 0;    rot[2] = -1;
   rot[3] =1;    rot[4] = 6.123233995736766e-17;    rot[5] = 6.123233995736766e-17;
   rot[6] =6.123233995736766e-17;    rot[7] = -1;    rot[8] = 3.749399456654644e-33;
   TGeoHMatrix *pMatrix150 = new TGeoHMatrix("");
   pMatrix150->SetTranslation(tr);
   pMatrix150->SetRotation(rot);
   pMatrix150->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix150->SetBit(TGeoMatrix::kGeoRotation);
   // HMatrix: 
   tr[0]  = 33.57;    tr[1] = 20;    tr[2] = -4.2000000000000002;
   rot[0] =6.123233995736766e-17;    rot[1] = 0;    rot[2] = -1;
   rot[3] =1;    rot[4] = 6.123233995736766e-17;    rot[5] = 6.123233995736766e-17;
   rot[6] =6.123233995736766e-17;    rot[7] = -1;    rot[8] = 3.749399456654644e-33;
   TGeoHMatrix *pMatrix151 = new TGeoHMatrix("");
   pMatrix151->SetTranslation(tr);
   pMatrix151->SetRotation(rot);
   pMatrix151->SetBit(TGeoMatrix::kGeoTranslation);
   pMatrix151->SetBit(TGeoMatrix::kGeoRotation);
   // Shape: WorldSolid type: TGeoBBox
   dx = 175;
   dy = 175;
   dz = 175;
   TGeoShape *pWorldSolid_1 = new TGeoBBox("WorldSolid", dx,dy,dz);
   // Volume: World
   pWorld_55d613a3b5c0 = new TGeoVolume("World",pWorldSolid_1, pMed17);
   pWorld_55d613a3b5c0->SetLineColor(593);
   pWorld_55d613a3b5c0->SetVisLeaves(kTRUE);

   // SET TOP VOLUME OF GEOMETRY
   gGeoManager->SetTopVolume(pWorld_55d613a3b5c0);

   // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
   // Shape: gasSolidFull type: TGeoTube
   rmin = 0;
   rmax = 4.8000000000000007;
   dz   = 1.5;
   TGeoShape *pgasSolidFull_2 = new TGeoTube("gasSolidFull",rmin,rmax,dz);
   // Shape: mylarCathode type: TGeoTube
   rmin = 0;
   rmax = 1.5;
   dz   = 0.00020000000000000001;
   TGeoShape *pmylarCathode_8 = new TGeoTube("mylarCathode",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 1.4998;
   pMatrix7 = new TGeoCombiTrans("");
   pMatrix7->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pgasSolidFull_2,pmylarCathode_8,0,pMatrix7);
   // Shape: gasSolid1 type: TGeoCompositeShape
   TGeoShape *pgasSolid1_28 = new TGeoCompositeShape("gasSolid1", pBoolNode);
   // Shape: kaptonReadout type: TGeoTube
   rmin = 0;
   rmax = 4.8000000000000007;
   dz   = 0.025000000000000001;
   TGeoShape *pkaptonReadout_3 = new TGeoTube("kaptonReadout",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = -1.5250000000000001;
   pMatrix9 = new TGeoCombiTrans("");
   pMatrix9->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pgasSolid1_28,pkaptonReadout_3,0,pMatrix9);
   // Shape: gasSolid2 type: TGeoCompositeShape
   TGeoShape *pgasSolid2_29 = new TGeoCompositeShape("gasSolid2", pBoolNode);
   // Shape: copperReadout type: TGeoBBox
   dx = 3;
   dy = 3;
   dz = 0.010000000000000002;
   TGeoShape *pcopperReadout_4 = new TGeoBBox("copperReadout", dx,dy,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = -1.5600000000000001;
   // Rotation: 
   thx = 90;    phx = 45;
   thy = 90;    phy = 135;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix12 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix11 = new TGeoCombiTrans("", dx,dy,dz,pMatrix12);
   pBoolNode = new TGeoSubtraction(pgasSolid2_29,pcopperReadout_4,0,pMatrix11);
   // Shape: gasSolid type: TGeoCompositeShape
   TGeoShape *pgasSolid_30 = new TGeoCompositeShape("gasSolid", pBoolNode);
   // Volume: gasVolume
   pgasVolume_55d6146ce9f0 = new TGeoVolume("gasVolume",pgasSolid_30, pMed9);
   pgasVolume_55d6146ce9f0->SetLineColor(920);
   pgasVolume_55d6146ce9f0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pgasVolume_55d6146ce9f0, 0, pMatrix113);
   // Volume: kaptonReadoutVolume
   pkaptonReadoutVolume_55d613a41aa0 = new TGeoVolume("kaptonReadoutVolume",pkaptonReadout_3, pMed20);
   pkaptonReadoutVolume_55d613a41aa0->SetLineColor(593);
   pkaptonReadoutVolume_55d613a41aa0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pkaptonReadoutVolume_55d613a41aa0, 0, pMatrix114);
   // Volume: copperReadoutVolume
   pcopperReadoutVolume_55d613a41d30 = new TGeoVolume("copperReadoutVolume",pcopperReadout_4, pMed13);
   pcopperReadoutVolume_55d613a41d30->SetLineColor(809);
   pcopperReadoutVolume_55d613a41d30->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pcopperReadoutVolume_55d613a41d30, 0, pMatrix115);
   // Shape: backplate type: TGeoTube
   rmin = 0;
   rmax = 6.8500000000000005;
   dz   = 0.60000000000000009;
   TGeoShape *pbackplate_5 = new TGeoTube("backplate",rmin,rmax,dz);
   // Volume: backplateVolume
   pbackplateVolume_55d6146922f0 = new TGeoVolume("backplateVolume",pbackplate_5, pMed13);
   pbackplateVolume_55d6146922f0->SetLineColor(809);
   pbackplateVolume_55d6146922f0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pbackplateVolume_55d6146922f0, 0, pMatrix116);
   // Shape: copperChamber type: TGeoTube
   rmin = 5;
   rmax = 6.8500000000000005;
   dz   = 2.25;
   TGeoShape *pcopperChamber_6 = new TGeoTube("copperChamber",rmin,rmax,dz);
   // Volume: copperChamberVolume
   pcopperChamberVolume_55d614692510 = new TGeoVolume("copperChamberVolume",pcopperChamber_6, pMed13);
   pcopperChamberVolume_55d614692510->SetLineColor(809);
   pcopperChamberVolume_55d614692510->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pcopperChamberVolume_55d614692510, 0, pMatrix117);
   // Shape: teflonCoverChamber type: TGeoTube
   rmin = 4.8000000000000007;
   rmax = 5;
   dz   = 2.25;
   TGeoShape *pteflonCoverChamber_7 = new TGeoTube("teflonCoverChamber",rmin,rmax,dz);
   // Volume: teflonCoverChamberVolume
   pteflonCoverChamberVolume_55d6146927d0 = new TGeoVolume("teflonCoverChamberVolume",pteflonCoverChamber_7, pMed21);
   pteflonCoverChamberVolume_55d6146927d0->SetLineColor(613);
   pteflonCoverChamberVolume_55d6146927d0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pteflonCoverChamberVolume_55d6146927d0, 0, pMatrix118);
   // Volume: mylarCathodeVolume
   pmylarCathodeVolume_55d613a41ff0 = new TGeoVolume("mylarCathodeVolume",pmylarCathode_8, pMed22);
   pmylarCathodeVolume_55d613a41ff0->SetLineColor(593);
   pmylarCathodeVolume_55d613a41ff0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pmylarCathodeVolume_55d613a41ff0, 0, pMatrix119);
   // Shape: copperCathode type: TGeoTube
   rmin = 1.5;
   rmax = 4.8000000000000007;
   dz   = 0.75;
   TGeoShape *pcopperCathode_9 = new TGeoTube("copperCathode",rmin,rmax,dz);
   // Volume: copperCathodeVolume
   pcopperCathodeVolume_55d614692ad0 = new TGeoVolume("copperCathodeVolume",pcopperCathode_9, pMed13);
   pcopperCathodeVolume_55d614692ad0->SetLineColor(809);
   pcopperCathodeVolume_55d614692ad0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pcopperCathodeVolume_55d614692ad0, 0, pMatrix120);
   // Shape: copperChamberTop type: TGeoTube
   rmin = 3;
   rmax = 6.8500000000000005;
   dz   = 0.125;
   TGeoShape *pcopperChamberTop_10 = new TGeoTube("copperChamberTop",rmin,rmax,dz);
   // Volume: copperChamberTopVolume
   pcopperChamberTopVolume_55d614692d60 = new TGeoVolume("copperChamberTopVolume",pcopperChamberTop_10, pMed13);
   pcopperChamberTopVolume_55d614692d60->SetLineColor(809);
   pcopperChamberTopVolume_55d614692d60->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pcopperChamberTopVolume_55d614692d60, 0, pMatrix121);
   // Shape: teflonCoverCathode type: TGeoTube
   rmin = 0.95000000000000007;
   rmax = 1.5;
   dz   = 0.72500000000000009;
   TGeoShape *pteflonCoverCathode_15 = new TGeoTube("teflonCoverCathode",rmin,rmax,dz);
   // Volume: teflonCoverCathodeVolume
   pteflonCoverCathodeVolume_55d6146935a0 = new TGeoVolume("teflonCoverCathodeVolume",pteflonCoverCathode_15, pMed21);
   pteflonCoverCathodeVolume_55d6146935a0->SetLineColor(613);
   pteflonCoverCathodeVolume_55d6146935a0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pteflonCoverCathodeVolume_55d6146935a0, 0, pMatrix122);
   // Shape: teflonCoverCathodeFilling type: TGeoTube
   rmin = 0;
   rmax = 0.95000000000000007;
   dz   = 0.72500000000000009;
   TGeoShape *pteflonCoverCathodeFilling_16 = new TGeoTube("teflonCoverCathodeFilling",rmin,rmax,dz);
   // Volume: teflonCoverCathodeFillingVolume
   pteflonCoverCathodeFillingVolume_55d6146938a0 = new TGeoVolume("teflonCoverCathodeFillingVolume",pteflonCoverCathodeFilling_16, pMed17);
   pteflonCoverCathodeFillingVolume_55d6146938a0->SetLineColor(593);
   pteflonCoverCathodeFillingVolume_55d6146938a0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pteflonCoverCathodeFillingVolume_55d6146938a0, 0, pMatrix123);
   // Shape: Ring2 type: TGeoTube
   rmin = 1.25;
   rmax = 1.5;
   dz   = 0.15000000000000002;
   TGeoShape *pRing2_22 = new TGeoTube("Ring2",rmin,rmax,dz);
   // Shape: line1 type: TGeoBBox
   dx = 1.25;
   dy = 0.10000000000000001;
   dz = 0.15000000000000002;
   TGeoShape *pline1_21 = new TGeoBBox("line1", dx,dy,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 22.500000000000004;
   thy = 90;    phy = 112.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix15 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix14 = new TGeoCombiTrans("", dx,dy,dz,pMatrix15);
   pBoolNode = new TGeoUnion(pRing2_22,pline1_21,0,pMatrix14);
   // Shape: cathodePattern1 type: TGeoCompositeShape
   TGeoShape *pcathodePattern1_31 = new TGeoCompositeShape("cathodePattern1", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 67.5;
   thy = 90;    phy = 157.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix18 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix17 = new TGeoCombiTrans("", dx,dy,dz,pMatrix18);
   pBoolNode = new TGeoUnion(pcathodePattern1_31,pline1_21,0,pMatrix17);
   // Shape: cathodePattern2 type: TGeoCompositeShape
   TGeoShape *pcathodePattern2_32 = new TGeoCompositeShape("cathodePattern2", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 112.5;
   thy = 90;    phy = 202.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix21 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix20 = new TGeoCombiTrans("", dx,dy,dz,pMatrix21);
   pBoolNode = new TGeoUnion(pcathodePattern2_32,pline1_21,0,pMatrix20);
   // Shape: cathodePattern3 type: TGeoCompositeShape
   TGeoShape *pcathodePattern3_33 = new TGeoCompositeShape("cathodePattern3", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 157.5;
   thy = 90;    phy = 247.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix24 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix23 = new TGeoCombiTrans("", dx,dy,dz,pMatrix24);
   pBoolNode = new TGeoUnion(pcathodePattern3_33,pline1_21,0,pMatrix23);
   // Shape: cathodePattern4 type: TGeoCompositeShape
   TGeoShape *pcathodePattern4_34 = new TGeoCompositeShape("cathodePattern4", pBoolNode);
   // Shape: Hole1 type: TGeoTube
   rmin = 0;
   rmax = 0.95000000000000007;
   dz   = 0.25;
   TGeoShape *pHole1_24 = new TGeoTube("Hole1",rmin,rmax,dz);
   pBoolNode = new TGeoSubtraction(pcathodePattern4_34,pHole1_24,0,0);
   // Shape: cathodePattern5 type: TGeoCompositeShape
   TGeoShape *pcathodePattern5_35 = new TGeoCompositeShape("cathodePattern5", pBoolNode);
   // Shape: Ring1 type: TGeoTube
   rmin = 0.85000000000000009;
   rmax = 0.95000000000000007;
   dz   = 0.15000000000000002;
   TGeoShape *pRing1_20 = new TGeoTube("Ring1",rmin,rmax,dz);
   pBoolNode = new TGeoUnion(pcathodePattern5_35,pRing1_20,0,0);
   // Shape: cathodePattern6 type: TGeoCompositeShape
   TGeoShape *pcathodePattern6_36 = new TGeoCompositeShape("cathodePattern6", pBoolNode);
   // Shape: line0 type: TGeoBBox
   dx = 1.25;
   dy = 0.014999999999999999;
   dz = 0.15000000000000002;
   TGeoShape *pline0_19 = new TGeoBBox("line0", dx,dy,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 22.500000000000004;
   thy = 90;    phy = 112.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix31 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix30 = new TGeoCombiTrans("", dx,dy,dz,pMatrix31);
   pBoolNode = new TGeoUnion(pcathodePattern6_36,pline0_19,0,pMatrix30);
   // Shape: cathodePattern7 type: TGeoCompositeShape
   TGeoShape *pcathodePattern7_37 = new TGeoCompositeShape("cathodePattern7", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 67.5;
   thy = 90;    phy = 157.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix34 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix33 = new TGeoCombiTrans("", dx,dy,dz,pMatrix34);
   pBoolNode = new TGeoUnion(pcathodePattern7_37,pline0_19,0,pMatrix33);
   // Shape: cathodePattern8 type: TGeoCompositeShape
   TGeoShape *pcathodePattern8_38 = new TGeoCompositeShape("cathodePattern8", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 112.5;
   thy = 90;    phy = 202.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix37 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix36 = new TGeoCombiTrans("", dx,dy,dz,pMatrix37);
   pBoolNode = new TGeoUnion(pcathodePattern8_38,pline0_19,0,pMatrix36);
   // Shape: cathodePattern9 type: TGeoCompositeShape
   TGeoShape *pcathodePattern9_39 = new TGeoCompositeShape("cathodePattern9", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 0;
   // Rotation: 
   thx = 90;    phx = 157.5;
   thy = 90;    phy = 247.5;
   thz = 0;    phz = 0;
   TGeoRotation *pMatrix40 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   pMatrix39 = new TGeoCombiTrans("", dx,dy,dz,pMatrix40);
   pBoolNode = new TGeoUnion(pcathodePattern9_39,pline0_19,0,pMatrix39);
   // Shape: cathodePattern10 type: TGeoCompositeShape
   TGeoShape *pcathodePattern10_40 = new TGeoCompositeShape("cathodePattern10", pBoolNode);
   // Shape: Hole0 type: TGeoTube
   rmin = 0;
   rmax = 0.45500000000000002;
   dz   = 0.25;
   TGeoShape *pHole0_23 = new TGeoTube("Hole0",rmin,rmax,dz);
   pBoolNode = new TGeoSubtraction(pcathodePattern10_40,pHole0_23,0,0);
   // Shape: cathodePattern11 type: TGeoCompositeShape
   TGeoShape *pcathodePattern11_41 = new TGeoCompositeShape("cathodePattern11", pBoolNode);
   // Shape: Ring0 type: TGeoTube
   rmin = 0.42500000000000004;
   rmax = 0.45500000000000002;
   dz   = 0.15000000000000002;
   TGeoShape *pRing0_18 = new TGeoTube("Ring0",rmin,rmax,dz);
   pBoolNode = new TGeoUnion(pcathodePattern11_41,pRing0_18,0,0);
   // Shape: cathodePattern type: TGeoCompositeShape
   TGeoShape *pcathodePattern_42 = new TGeoCompositeShape("cathodePattern", pBoolNode);
   // Volume: cathodePatternVolume
   pcathodePatternVolume_55d614693e00 = new TGeoVolume("cathodePatternVolume",pcathodePattern_42, pMed13);
   pcathodePatternVolume_55d614693e00->SetLineColor(809);
   pcathodePatternVolume_55d614693e00->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pcathodePatternVolume_55d614693e00, 0, pMatrix124);
   // Shape: vacuumCathode type: TGeoTube
   rmin = 0;
   rmax = 1.25;
   dz   = 0.15000000000000002;
   TGeoShape *pvacuumCathode_25 = new TGeoTube("vacuumCathode",rmin,rmax,dz);
   pBoolNode = new TGeoSubtraction(pvacuumCathode_25,pcathodePattern_42,0,0);
   // Shape: vacuumCathodePattern type: TGeoCompositeShape
   TGeoShape *pvacuumCathodePattern_43 = new TGeoCompositeShape("vacuumCathodePattern", pBoolNode);
   // Volume: vacuumCathodePatternVolume
   pvacuumCathodePatternVolume_55d6146940c0 = new TGeoVolume("vacuumCathodePatternVolume",pvacuumCathodePattern_43, pMed17);
   pvacuumCathodePatternVolume_55d6146940c0->SetLineColor(593);
   pvacuumCathodePatternVolume_55d6146940c0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pvacuumCathodePatternVolume_55d6146940c0, 0, pMatrix125);
   // Shape: teflonCylinder1of4 type: TGeoTube
   rmin = 1;
   rmax = 2.1499999999999999;
   dz   = 1.1950000000000001;
   TGeoShape *pteflonCylinder1of4_44 = new TGeoTube("teflonCylinder1of4",rmin,rmax,dz);
   // Shape: teflonCylinder2of4 type: TGeoTube
   rmin = 1.2000000000000002;
   rmax = 2.1499999999999999;
   dz   = 1.4199999999999999;
   TGeoShape *pteflonCylinder2of4_45 = new TGeoTube("teflonCylinder2of4",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 2.6150000000000002;
   pMatrix48 = new TGeoCombiTrans("");
   pMatrix48->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pteflonCylinder1of4_44,pteflonCylinder2of4_45,0,pMatrix48);
   // Shape: teflonPipeSolid_v1 type: TGeoCompositeShape
   TGeoShape *pteflonPipeSolid_v1_48 = new TGeoCompositeShape("teflonPipeSolid_v1", pBoolNode);
   // Shape: teflonCylinder3of4 type: TGeoTube
   rmin = 1.5;
   rmax = 2.1499999999999999;
   dz   = 2.0100000000000002;
   TGeoShape *pteflonCylinder3of4_46 = new TGeoTube("teflonCylinder3of4",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 6.0449999999999999;
   pMatrix50 = new TGeoCombiTrans("");
   pMatrix50->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pteflonPipeSolid_v1_48,pteflonCylinder3of4_46,0,pMatrix50);
   // Shape: teflonPipeSolid_v2 type: TGeoCompositeShape
   TGeoShape *pteflonPipeSolid_v2_49 = new TGeoCompositeShape("teflonPipeSolid_v2", pBoolNode);
   // Shape: teflonCylinder4of4 type: TGeoTube
   rmin = 1.6500000000000001;
   rmax = 2.1499999999999999;
   dz   = 1.02;
   TGeoShape *pteflonCylinder4of4_47 = new TGeoTube("teflonCylinder4of4",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 9.0750000000000011;
   pMatrix52 = new TGeoCombiTrans("");
   pMatrix52->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pteflonPipeSolid_v2_49,pteflonCylinder4of4_47,0,pMatrix52);
   // Shape: teflonPipeSolid type: TGeoCompositeShape
   TGeoShape *pteflonPipeSolid_50 = new TGeoCompositeShape("teflonPipeSolid", pBoolNode);
   // Volume: teflonPipeInCopper
   pteflonPipeInCopper_55d614694680 = new TGeoVolume("teflonPipeInCopper",pteflonPipeSolid_50, pMed21);
   pteflonPipeInCopper_55d614694680->SetLineColor(613);
   pteflonPipeInCopper_55d614694680->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pteflonPipeInCopper_55d614694680, 0, pMatrix126);
   // Shape: copperPipeDetectorFlangeTube type: TGeoTube
   rmin = 0;
   rmax = 6.75;
   dz   = 0.85000000000000009;
   TGeoShape *pcopperPipeDetectorFlangeTube_51 = new TGeoTube("copperPipeDetectorFlangeTube",rmin,rmax,dz);
   // Shape: copperPipeDetectorToCentralTube type: TGeoTube
   rmin = 0;
   rmax = 4;
   dz   = 2.25;
   TGeoShape *pcopperPipeDetectorToCentralTube_52 = new TGeoTube("copperPipeDetectorToCentralTube",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 3.1000000000000001;
   pMatrix54 = new TGeoCombiTrans("");
   pMatrix54->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pcopperPipeDetectorFlangeTube_51,pcopperPipeDetectorToCentralTube_52,0,pMatrix54);
   // Shape: copperPipeSolid_v1 type: TGeoCompositeShape
   TGeoShape *pcopperPipeSolid_v1_53 = new TGeoCompositeShape("copperPipeSolid_v1", pBoolNode);
   // Shape: copperPipeCentralPieceSolid_v1 type: TGeoTube
   rmin = 0;
   rmax = 7.5;
   dz   = 17.75;
   TGeoShape *pcopperPipeCentralPieceSolid_v1_54 = new TGeoTube("copperPipeCentralPieceSolid_v1",rmin,rmax,dz);
   // Shape: copperPipeCentralPieceBox_v1 type: TGeoBBox
   dx = 7.875;
   dy = 7.875;
   dz = 18.25;
   TGeoShape *pcopperPipeCentralPieceBox_v1_55 = new TGeoBBox("copperPipeCentralPieceBox_v1", dx,dy,dz);
   // Shape: copperPipeCentralPieceBoxToSubs type: TGeoBBox
   dx = 5.9500000000000002;
   dy = 5.9500000000000002;
   dz = 18.25;
   TGeoShape *pcopperPipeCentralPieceBoxToSubs_56 = new TGeoBBox("copperPipeCentralPieceBoxToSubs", dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pcopperPipeCentralPieceBox_v1_55,pcopperPipeCentralPieceBoxToSubs_56,0,0);
   // Shape: copperPipeCentralPieceBox type: TGeoCompositeShape
   TGeoShape *pcopperPipeCentralPieceBox_57 = new TGeoCompositeShape("copperPipeCentralPieceBox", pBoolNode);
   pBoolNode = new TGeoSubtraction(pcopperPipeCentralPieceSolid_v1_54,pcopperPipeCentralPieceBox_57,0,0);
   // Shape: copperPipeCentralPieceSolid_v2 type: TGeoCompositeShape
   TGeoShape *pcopperPipeCentralPieceSolid_v2_58 = new TGeoCompositeShape("copperPipeCentralPieceSolid_v2", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 23.100000000000001;
   pMatrix60 = new TGeoCombiTrans("");
   pMatrix60->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pcopperPipeSolid_v1_53,pcopperPipeCentralPieceSolid_v2_58,0,pMatrix60);
   // Shape: copperPipeSolid_v2 type: TGeoCompositeShape
   TGeoShape *pcopperPipeSolid_v2_59 = new TGeoCompositeShape("copperPipeSolid_v2", pBoolNode);
   // Shape: copperPipeCentralToOutTube type: TGeoTube
   rmin = 0;
   rmax = 5.4000000000000004;
   dz   = 2.9500000000000002;
   TGeoShape *pcopperPipeCentralToOutTube_60 = new TGeoTube("copperPipeCentralToOutTube",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 43.800000000000004;
   pMatrix62 = new TGeoCombiTrans("");
   pMatrix62->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pcopperPipeSolid_v2_59,pcopperPipeCentralToOutTube_60,0,pMatrix62);
   // Shape: copperPipeSolid_v3 type: TGeoCompositeShape
   TGeoShape *pcopperPipeSolid_v3_62 = new TGeoCompositeShape("copperPipeSolid_v3", pBoolNode);
   // Shape: copperPipeOutFlangeTube type: TGeoTube
   rmin = 0;
   rmax = 7.5;
   dz   = 0.95000000000000007;
   TGeoShape *pcopperPipeOutFlangeTube_61 = new TGeoTube("copperPipeOutFlangeTube",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 47.700000000000003;
   pMatrix64 = new TGeoCombiTrans("");
   pMatrix64->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pcopperPipeSolid_v3_62,pcopperPipeOutFlangeTube_61,0,pMatrix64);
   // Shape: copperPipeSolid_v4 type: TGeoCompositeShape
   TGeoShape *pcopperPipeSolid_v4_63 = new TGeoCompositeShape("copperPipeSolid_v4", pBoolNode);
   // Shape: copperPipeInsideCylinder1Solid type: TGeoTube
   rmin = 0;
   rmax = 2.1499999999999999;
   dz   = 13.125250000000001;
   TGeoShape *pcopperPipeInsideCylinder1Solid_64 = new TGeoTube("copperPipeInsideCylinder1Solid",rmin,rmax,dz);
   // Shape: copperPipeInsideConeUnion1Solid type: TGeoCone
   dz    = 0.625;
   rmin1 = 0;
   rmax1 = 2.1499999999999999;
   rmin2 = 0;
   rmax2 = 3.4000000000000004;
   TGeoShape *pcopperPipeInsideConeUnion1Solid_65 = new TGeoCone("copperPipeInsideConeUnion1Solid", dz,rmin1,rmax1,rmin2,rmax2);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 13.750250000000001;
   pMatrix66 = new TGeoCombiTrans("");
   pMatrix66->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pcopperPipeInsideCylinder1Solid_64,pcopperPipeInsideConeUnion1Solid_65,0,pMatrix66);
   // Shape: copperPipeInsideSolid_v1 type: TGeoCompositeShape
   TGeoShape *pcopperPipeInsideSolid_v1_68 = new TGeoCompositeShape("copperPipeInsideSolid_v1", pBoolNode);
   // Shape: copperPipeInsideCylinder2Solid type: TGeoTube
   rmin = 0;
   rmax = 3.4000000000000004;
   dz   = 10.5;
   TGeoShape *pcopperPipeInsideCylinder2Solid_66 = new TGeoTube("copperPipeInsideCylinder2Solid",rmin,rmax,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 24.875250000000001;
   pMatrix68 = new TGeoCombiTrans("");
   pMatrix68->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pcopperPipeInsideSolid_v1_68,pcopperPipeInsideCylinder2Solid_66,0,pMatrix68);
   // Shape: copperPipeInsideSolid_v2 type: TGeoCompositeShape
   TGeoShape *pcopperPipeInsideSolid_v2_69 = new TGeoCompositeShape("copperPipeInsideSolid_v2", pBoolNode);
   // Shape: copperPipeInsideConeUnion2Solid type: TGeoCone
   dz    = 0.50025000000000008;
   rmin1 = 0;
   rmax1 = 3.4000000000000004;
   rmin2 = 0;
   rmax2 = 4.4000000000000004;
   TGeoShape *pcopperPipeInsideConeUnion2Solid_67 = new TGeoCone("copperPipeInsideConeUnion2Solid", dz,rmin1,rmax1,rmin2,rmax2);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 35.875500000000002;
   pMatrix70 = new TGeoCombiTrans("");
   pMatrix70->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoUnion(pcopperPipeInsideSolid_v2_69,pcopperPipeInsideConeUnion2Solid_67,0,pMatrix70);
   // Shape: copperPipeInsideSolid type: TGeoCompositeShape
   TGeoShape *pcopperPipeInsideSolid_70 = new TGeoCompositeShape("copperPipeInsideSolid", pBoolNode);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 12.274750000000001;
   pMatrix76 = new TGeoCombiTrans("");
   pMatrix76->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pcopperPipeSolid_v4_63,pcopperPipeInsideSolid_70,0,pMatrix76);
   // Shape: copperPipeCentralPieceSolid type: TGeoCompositeShape
   TGeoShape *pcopperPipeCentralPieceSolid_73 = new TGeoCompositeShape("copperPipeCentralPieceSolid", pBoolNode);
   // Volume: detectorCopperPipe
   pdetectorCopperPipe_55d6146943c0 = new TGeoVolume("detectorCopperPipe",pcopperPipeCentralPieceSolid_73, pMed13);
   pdetectorCopperPipe_55d6146943c0->SetLineColor(809);
   pdetectorCopperPipe_55d6146943c0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pdetectorCopperPipe_55d6146943c0, 0, pMatrix127);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = -11.930250000000001;
   pMatrix74 = new TGeoCombiTrans("");
   pMatrix74->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pcopperPipeInsideSolid_70,pteflonPipeSolid_50,0,pMatrix74);
   // Shape: pipeFillingSolid type: TGeoCompositeShape
   TGeoShape *ppipeFillingSolid_72 = new TGeoCompositeShape("pipeFillingSolid", pBoolNode);
   // Volume: pipeFilling
   ppipeFilling_55d614694940 = new TGeoVolume("pipeFilling",ppipeFillingSolid_72, pMed17);
   ppipeFilling_55d614694940->SetLineColor(593);
   ppipeFilling_55d614694940->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(ppipeFilling_55d614694940, 0, pMatrix128);
   // Shape: leadBoxSolid type: TGeoBBox
   dx = 29.5;
   dy = 29.5;
   dz = 27;
   TGeoShape *pleadBoxSolid_77 = new TGeoBBox("leadBoxSolid", dx,dy,dz);
   // Shape: leadBoxShaft type: TGeoBBox
   dx = 9.7000000000000011;
   dy = 8.5;
   dz = 17.050000000000001;
   TGeoShape *pleadBoxShaft_78 = new TGeoBBox("leadBoxShaft", dx,dy,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = 10.050000000000001;
   pMatrix82 = new TGeoCombiTrans("");
   pMatrix82->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pleadBoxSolid_77,pleadBoxShaft_78,0,pMatrix82);
   // Shape: leadBoxWithShaft type: TGeoCompositeShape
   TGeoShape *pleadBoxWithShaft_79 = new TGeoCompositeShape("leadBoxWithShaft", pBoolNode);
   // Volume: leadBoxVolume
   pleadBoxVolume_55d613a39550 = new TGeoVolume("leadBoxVolume",pleadBoxWithShaft_79, pMed19);
   pleadBoxVolume_55d613a39550->SetLineColor(920);
   pleadBoxVolume_55d613a39550->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pleadBoxVolume_55d613a39550, 0, pMatrix129);
   // Shape: leadPipeCollar type: TGeoBBox
   dx = 16;
   dy = 14.5;
   dz = 2.5;
   TGeoShape *pleadPipeCollar_80 = new TGeoBBox("leadPipeCollar", dx,dy,dz);
   // Combi transformation: 
   dx = 0;
   dy = 0;
   dz = -20;
   pMatrix84 = new TGeoCombiTrans("");
   pMatrix84->SetTranslation(dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pleadPipeCollar_80,pcopperPipeSolid_v4_63,0,pMatrix84);
   // Shape: leadPipeCollarHollow type: TGeoCompositeShape
   TGeoShape *pleadPipeCollarHollow_81 = new TGeoCompositeShape("leadPipeCollarHollow", pBoolNode);
   // Volume: leadCollar
   pleadCollar_55d613a397c0 = new TGeoVolume("leadCollar",pleadPipeCollarHollow_81, pMed19);
   pleadCollar_55d613a397c0->SetLineColor(920);
   pleadCollar_55d613a397c0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pleadCollar_55d613a397c0, 0, pMatrix130);
   // Shape: scintillatorBoxSolid type: TGeoBBox
   dx = 9.990000000000002;
   dy = 34.990000000000002;
   dz = 2.5;
   TGeoShape *pscintillatorBoxSolid_114 = new TGeoBBox("scintillatorBoxSolid", dx,dy,dz);
   // Volume: scintillatorVolume
   pscintillatorVolume_55d613a3a210 = new TGeoVolume("scintillatorVolume",pscintillatorBoxSolid_114, pMed23);
   pscintillatorVolume_55d613a3a210->SetLineColor(406);
   pscintillatorVolume_55d613a3a210->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix131);
   // Shape: scintillatorHoleSquare type: TGeoBBox
   dx = 10.005000000000001;
   dy = 10.005000000000001;
   dz = 3;
   TGeoShape *pscintillatorHoleSquare_117 = new TGeoBBox("scintillatorHoleSquare", dx,dy,dz);
   pBoolNode = new TGeoSubtraction(pscintillatorBoxSolid_114,pscintillatorHoleSquare_117,0,0);
   // Shape: scintillatorBoxSolidWithMiddleHole type: TGeoCompositeShape
   TGeoShape *pscintillatorBoxSolidWithMiddleHole_120 = new TGeoCompositeShape("scintillatorBoxSolidWithMiddleHole", pBoolNode);
   // Volume: scintillatorVolumeWithMiddleHole
   pscintillatorVolumeWithMiddleHole_55d613a3a4d0 = new TGeoVolume("scintillatorVolumeWithMiddleHole",pscintillatorBoxSolidWithMiddleHole_120, pMed23);
   pscintillatorVolumeWithMiddleHole_55d613a3a4d0->SetLineColor(406);
   pscintillatorVolumeWithMiddleHole_55d613a3a4d0->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolumeWithMiddleHole_55d613a3a4d0, 0, pMatrix132);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix133);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix134);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix135);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix136);
   // Shape: scintillatorSheetSolid1 type: TGeoBBox
   dx = 9.990000000000002;
   dy = 34.990000000000002;
   dz = 5.0000000000000002e-05;
   TGeoShape *pscintillatorSheetSolid1_111 = new TGeoBBox("scintillatorSheetSolid1", dx,dy,dz);
   // Volume: scintillatorSheetVolume1
   pscintillatorSheetVolume1_55d613a3ad20 = new TGeoVolume("scintillatorSheetVolume1",pscintillatorSheetSolid1_111, pMed17);
   pscintillatorSheetVolume1_55d613a3ad20->SetLineColor(593);
   pscintillatorSheetVolume1_55d613a3ad20->SetVisLeaves(kTRUE);
   pWorld_55d613a3b5c0->AddNode(pscintillatorSheetVolume1_55d613a3ad20, 0, pMatrix137);
   pWorld_55d613a3b5c0->AddNode(pscintillatorSheetVolume1_55d613a3ad20, 0, pMatrix138);
   pWorld_55d613a3b5c0->AddNode(pscintillatorSheetVolume1_55d613a3ad20, 0, pMatrix139);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix140);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix141);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix142);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix143);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix144);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix145);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix146);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix147);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix148);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix149);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix150);
   pWorld_55d613a3b5c0->AddNode(pscintillatorVolume_55d613a3a210, 0, pMatrix151);

   // CLOSE GEOMETRY
   gGeoManager->CloseGeometry();
}
