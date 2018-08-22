#!/usr/bin/octave -qf

% ---- Variables:

fps = 24;
buffsize = 100;
prebuffperc = 50;
tmax = 10;

% ---- Code:

graphics_toolkit('qt');

% ---- FIFO:

global fifo;
fifo = [];
fifo.data = cell(1,buffsize);
fifo.start = 1;
fifo.final = 1;
fifo.bsize = buffsize;

function b = pull()
	global fifo;
	if(fsize > 0)
		b = fifo.data{mod(fifo.start,fifo.bsize)+1};
		fifo.start = fifo.start + 1;
	else
		b = NaN;
	endif
endfunction

function push(b)
	global fifo;
	fifo.data{mod(fifo.final,fifo.bsize)+1} = b;
	fifo.final = fifo.final + 1;
endfunction

function n = fsize()
	global fifo;
	n = fifo.final - fifo.start;
endfunction

% ---- Initial:

frame = 0;
fcount = 0;
rawdata = -1;
prebuff = 0;
olddata = [];

[ctrlh,outh,sim_pid] = popen2(argv(){end}); % open process

do % get size
	fprintf(ctrlh,'l'); fflush(ctrlh);
	l = fscanf(outh,'%d',2);
	fclear(outh);
	pause(0.1);
until l ~= -1
l = l(:)';

for i = 1:buffsize
	fifo.data{i} = zeros(l); % alocate buffer
end

% ---- Figure:

figure();

h = imshow(ones(l)); % open figure

pause(0.001);

% ---- Main loop:

basetime = tic;
do
	% prebuffer?
	if((fsize == 0) && (prebuff == 0))
		prebuff = round(prebuffperc*buffsize/100);
		title buffering
		pause(0.0001)
	% plot data if available and not prebuffering
	elseif( ( (toc(basetime)*fps) > frame) && (prebuff == 0) )
		frame = frame + 1;
		fcount = fcount + 1;
		% -- plotting:
		set(h,'cdata',pull());
		pause(1e-5);
	endif

	% get data
	if(isequal(rawdata, -1))
		fprintf(ctrlh,'p'); fflush(ctrlh);
		rawdata = fgetl(outh);
		fclear(outh);
	% clean bad data
	elseif(length(rawdata) < l(1)*l(2))
		rawdata = -1;
	% add data to buffer
	elseif((length(rawdata) >= l(1)*l(2)) && (fsize < buffsize-2))
		fprintf(ctrlh,'u'); fflush(ctrlh);
		rawdata((rawdata != '+') || (rawdata != '-')) = [];
		rawdata = reshape(rawdata == '+',l);

		if(!isequal(olddata, rawdata)) % ignore if repeated
			push(rawdata);
		endif
		olddata = rawdata;
		rawdata = -1;

		if(prebuff > 0)
			prebuff = prebuff - 1;
			if prebuff == 0
				basetime = tic;
				frame = 0;
				title running
			endif
		endif
	endif
until(fcount >= tmax*fps)

% ---- Close:

system(['kill -9 ' num2str(sim_pid)]);
