#!/bin/bash
FILENAME=$1
rm -rf oshpark
mkdir oshpark
echo $FILENAME-
cp $FILENAME.drl oshpark/$FILENAME.xln
cp $FILENAME\-F.Mask.g* oshpark/$FILENAME.GTS
cp $FILENAME\-B.Mask.g* oshpark/$FILENAME.GBS
cp $FILENAME\-F.Cu.g* oshpark/$FILENAME.GTL
cp $FILENAME\-B.Cu.g* oshpark/$FILENAME.GBL
cp $FILENAME\-In1.Cu.g* oshpark/$FILENAME.G2L
cp $FILENAME\-In2.Cu.g* oshpark/$FILENAME.G3L
cp $FILENAME\-F.SilkS.g* oshpark/$FILENAME.GTO
cp $FILENAME\-B.SilkS.g* oshpark/$FILENAME.GBO
cp $FILENAME\-Edge.Cuts.g* oshpark/$FILENAME.GKO
cd oshpark
zip ../$FILENAME.zip *
cd ..
rm -rf oshpark
