#!/usr/bin/octave -qf

% ---- TODO:

% automatic l

% ---- Variables:

fps = 24;
maxt = 10;
buffsize = 20;

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

figure();
h = imshow(ones(l));

data = zeros(l,l,buffsize); % FIFO buffer
kbuff = 1; % start buffer index (data out)
mbuff = 1; % end buffer index (data in)
oldframe = 0;
frame = 0;
rawdata = -1;

% ---- Main loop:

fprintf(ctrlh,'pu'); fflush(ctrlh);
tic;

do
	% get data
	if(rawdata == -1)
		rawdata = fgetl(outh);
		fclear(outh);
	end

	% add data to buffer
	if((length(rawdata) >= l(1)*l(2)) && (mod(mbuff+1,buffsize) != kbuff))
		rawdata((rawdata != '+') || (rawdata != '-')) = [];
		rawdata = reshape(rawdata == '+',l);

		mbuff = mod(mbuff+1,buffsize);
		data(:,:,mbuff+1) = rawdata;

		fprintf(ctrlh,'pu'); fflush(ctrlh);
		rawdata = -1;
	endif

	% plot data
	frame = round(toc*fps) + 1;
	if((frame > oldframe) && (kbuff != mbuff))
		oldframe = frame;
		set(h,'cdata',data(:,:,kbuff+1));
		kbuff = mod(kbuff+1,buffsize);
		pause(0.01);
	endif

	% clear invalid data
	if(!isequal(rawdata, -1) && length(rawdata) < l(1)*l(2))
		rawdata = -1;
	endif
until frame >= maxt*fps

system(['kill -9 ' num2str(sim_pid)]);

% save octave-workspace
