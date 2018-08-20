#!/usr/bin/octave -qf
graphics_toolkit('qt');

[ctrlh,outh,sim_pid] = popen2('./demos/a.out');

n = 240;
l = 400;

data = zeros(l,l,n);

k=1;
do
	fwrite(ctrlh,"\n"); fflush(ctrlh);

	rawdata = fgetl(outh);
	fclear(outh);

	if(rawdata ~= -1)
		data(:,:,k) = reshape(rawdata == '+',[l,l]);
		k = k + 1;
	endif
until k == n

figure();
h=imshow(zeros(256,256));

tic;
while(toc < 10)
	frame = mod(round(toc*24), size(data,3)) + 1;
	set(h,'cdata',data(:,:, frame));
	pause(0.01);
end
