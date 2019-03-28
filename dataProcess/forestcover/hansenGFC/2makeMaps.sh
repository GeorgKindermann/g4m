nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 treecover2010HansenV3.tif treeCov2010HansV3HDeg.tif &

#Land=1 other=0
nohup nice -19 python3 /usr/bin/gdal_calc.py --calc="where(A==1,1,0)" --type=Byte --co="TILED=YES" --co="COMPRESS=DEFLATE" --co="BIGTIFF=YES" --outfile=landHansenv15.tif -A landWaterHansenV16.tif &

#Create defor map compareable with Aformap
nohup nice -19 python3 /usr/bin/gdal_calc.py --calc="where(logical_and(A>0, A<13), 1, 0)" --type=Byte --co="TILED=YES" --co="COMPRESS=DEFLATE" --co="BIGTIFF=YES" --outfile=treecoverLoss20002012HansenV16.tif -A treecoverlossyearHansenV16.tif &

wait

nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 treecover2000HansenV16.tif treeCov2000HansV16HDeg.tif &
nohup nice -19 gdalwarp -wt Float32 -ot Float32 -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 landHansenv16.tif landHansV16HDeg.tif &
nohup nice -19 gdalwarp -wt Float32 -ot Float32 -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 treecoverGain20002012HansenV16.tif treeCovGain20002012HansV16HDeg.tif &
nohup nice -19 gdalwarp -wt Float32 -ot Float32 -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 treecoverLoss20002012HansenV16.tif treecoverLoss20002012HansV16HDeg.tif &

wait

nohup nice -19 python3 /usr/bin/gdal_calc.py --calc="minimum(100,maximum(0,A+75*B/12.*5.-where(logical_and(C>0,C<=5),A,0)))" --type=Byte --co="TILED=YES" --co="COMPRESS=DEFLATE" --co="BIGTIFF=YES" --outfile=treeCov2005HansV16Est.tif -A treecover2000HansenV16.tif -B treecoverGain20002012HansenV16.tif -C treecoverlossyearHansenV16.tif &

nohup nice -19 python3 /usr/bin/gdal_calc.py --calc="minimum(100,maximum(0,A+75*B/12.*10.-where(logical_and(C>0,C<=10),A,0)))" --type=Byte --co="TILED=YES" --co="COMPRESS=DEFLATE" --co="BIGTIFF=YES" --outfile=treeCov2010HansV16Est.tif -A treecover2000HansenV16.tif -B treecoverGain20002012HansenV16.tif -C treecoverlossyearHansenV16.tif &

nohup nice -19 python3 /usr/bin/gdal_calc.py --calc="minimum(100,maximum(0,A+75*B/12.*15.-where(logical_and(C>0,C<=15),A,0)))" --type=Byte --co="TILED=YES" --co="COMPRESS=DEFLATE" --co="BIGTIFF=YES" --outfile=treeCov2015HansV16Est.tif -A treecover2000HansenV16.tif -B treecoverGain20002012HansenV16.tif -C treecoverlossyearHansenV16.tif &

wait

nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 treeCov2005HansV16Est.tif treeCov2005HansV16EstHDeg.tif &
nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 treeCov2010HansV16Est.tif treeCov2010HansV16EstHDeg.tif &
nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" -r average -ts 43200 21600 treeCov2015HansV16Est.tif treeCov2015HansV16EstHDeg.tif &


#Management intensity
nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -r average -ts 1440 720 treeCov2000HansV16HDeg.tif treeCov2000_15Min.tif &
nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -r average -ts 1440 720 treeCovGain20002012HansV16HDeg.tif treeCovGain20002012_15Min.tif &
nohup nice -19 gdalwarp -wo NUM_THREADS=8 -multi -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -r average -ts 1440 720 treecoverLoss20002012HansV16HDeg.tif treecoverLoss20002012_15Min.tif &
wait
nohup nice -19 python3 /usr/bin/gdal_calc.py --NoDataValue=255 --calc="where(A>0,around(100*minimum(4e-5,minimum(B,C)/(A+B)/12.)/4e-5),0)" --type=Byte --co="TILED=YES" --co="COMPRESS=DEFLATE" --co="BIGTIFF=YES" --outfile=forestManagementIntensity15Min.tif -A treeCov2000_15Min.tif -B treeCovGain20002012_15Min.tif -C treecoverLoss20002012_15Min.tif
nohup nice -19 gdalwarp -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -co "TILED=YES" -r bilinear -ts 43200 21600 forestManagementIntensity15Min.tif forestManagementIntensity30Sec.tif
