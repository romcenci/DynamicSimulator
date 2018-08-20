#!/usr/bin/octave -qf

graphics_toolkit('qt');

[ctrlh,outh,sim_pid] = popen2('./demos/a.out');

zerodata = [];
data = [];

for i = 1:100
	disp(i);
	fwrite(ctrlh,"\n"); fflush(ctrlh);
	rawdata = fgetl(outh);
	fclear(outh);

	if(rawdata ~= -1)
		rawdata = str2num(rawdata);

		if isempty(zerodata)
			zerodata = 0*rawdata;
		end

		if size(rawdata) == size(zerodata)
			data(:,:,i) = reshape(rawdata,[1,1]*sqrt(length(rawdata)));
		end
	else
		pause(0.1);
	endif
end

figure();
h=imshow(zeros(256,256));

tic;
while(toc < 10)
	frame = mod(round(toc*24), size(data,3)) + 1
	set(h,'cdata',255*(data(:,:, frame)+1)/2);
pause(0.01);
end