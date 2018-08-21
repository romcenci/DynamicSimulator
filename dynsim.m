#!/usr/bin/octave -qf

% ---- TODO:

% automatic l

% ---- Variables:

fps = 24;
buffsize = 100;
prebuffperc = 50;
tmax = 10;

% ---- Code:

graphics_toolkit('qt');

[ctrlh,outh,sim_pid] = popen2(argv(){1}); % open process

do % get size
	fprintf(ctrlh,'l'); fflush(ctrlh);
	l = fscanf(outh,'%d',2);
	fclear(outh);
	pause(0.1);
until l ~= -1

l = l(:)';

h = imshow(ones(l));
pause(0.001);

data(1:buffsize) = mat2cell(zeros(l),l(1),l(2)); % FIFO buffer
cellzero = data(1);
buffn = 0;

frame = 0;
fcount = 0;

rawdata = -1;
prebuff = 0;

% ---- Main loop:
basetime = tic;
do
	% get data
	if(rawdata == -1)
		fprintf(ctrlh,'p'); fflush(ctrlh);
		rawdata = fgetl(outh);
		fclear(outh);
	% clean bad data
	elseif(length(rawdata) < l(1)*l(2))
		rawdata = -1;
	% add data to buffer
	elseif((length(rawdata) >= l(1)*l(2)) && buffn < buffsize)
		rawdata((rawdata != '+') || (rawdata != '-')) = [];
		rawdata = reshape(rawdata == '+',l);

		data{buffn + 1} = rawdata;
		buffn = buffn + 1;

		fprintf(ctrlh,'pu'); fflush(ctrlh);
		rawdata = -1;

		if(prebuff > 0)
			prebuff = prebuff - 1;
			if prebuff == 0
				basetime = tic;
				frame = 0;
			endif
		endif
	endif

	% prebuffer?
	if((buffn == 0) && (prebuff == 0))
		prebuff = round(prebuffperc*buffsize/100);
	% plot data
	elseif( ( (toc(basetime)*fps) > frame) && (prebuff == 0) )
		frame = frame + 1;
		fcount = fcount + 1;
		set(h,'cdata',data{1});
		pause(1e-5);

		data = [data(2:end) cellzero];
		buffn = buffn - 1;
	endif

until(fcount >= tmax*fps)

system(['kill -9 ' num2str(sim_pid)]);

% save octave-workspace
