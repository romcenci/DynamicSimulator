#!/usr/bin/octave -qf

% ---- Variables:

fps = 24;
buffsize = 10;
prebuffperc = 50;
tmax = 10;
superspeedval = 10;

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

function clearfifo()
	global fifo;
	fifo.data = cell(1,fifo.bsize);
	fifo.start = 1;
	fifo.final = 1;
endfunction

% ---- Initial:

frame = 0;
fcount = 0;
rawdata = -1;
prebuff = 0;
olddata = [];
global killflag = 0;
global pauseflag = 1;
global speedflag = 1;

global ctrlh outh sim_pid;
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

function playpause(h)
	global pauseflag;
	if(pauseflag)
		set(h,'label','Pause');
		pauseflag = 0;
	else
		set(h,'label','Start');
		pauseflag = 1;
	endif
endfunction

function resetsim()
	global ctrlh outh sim_pid;
	system(['kill -9 ' num2str(sim_pid)]);
	clearfifo();
	[ctrlh,outh,sim_pid] = popen2(argv(){end});
endfunction

function superspeed()
	global speedflag;
	if(speedflag == 1)
		speedflag = 10;
	else
		speedflag = 1;
	endif
endfunction

f = figure('menubar','none','toolbar','figure','units','pixels','name','DynamicSimulator',...
	'numbertitle','off','closerequestfcn','killflag=1;');

f1 = uimenu ('label', '&File','accelerator', 'f');
f11 = uimenu (f1, 'label', 'Autoscale', 'accelerator', 'a', ...
           'callback', 'axis([0,l(1),0,l(2)])');
f11 = uimenu (f1, 'label', 'Close', 'accelerator', 'q', ...
           'callback', 'killflag=1;');

f2 = uimenu ('label', '&Simulation','accelerator', 'a');
f21 = uimenu (f2, 'label', 'Start', 'accelerator', 's', ...
           'callback', 'playpause(f21);');
f22 = uimenu (f2, 'label', 'Superspeed', 'accelerator', 'v', ...
           'callback', 'superspeed();');
f23 = uimenu (f2, 'label', 'Reset', 'accelerator', 'r', ...
           'callback', 'resetsim();');

f3 = uimenu ('label', '&Options', 'accelerator', 's');

h = imshow(ones(l));

pause(0.001);

% ---- Main loop:

playpause(f21);

basetime = tic;
do
	% prebuffer?
	if((fsize == 0) && (prebuff == 0))
		prebuff = round(prebuffperc*buffsize/100);
		title buffering;
		pause(0.0001);
	% plot data if available and not prebuffering and not paused
	elseif( ( (toc(basetime)*fps) > frame) && (prebuff == 0) && !pauseflag)
		frame = frame + 1;
		fcount = fcount + 1;
		% -- plotting:
		title running
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
		for i = 1:speedflag
			fprintf(ctrlh,'u');
		endfor
		fflush(ctrlh);

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
				title running;
			endif
		endif
	endif

	if(pauseflag)
		title paused;
		waitfor(f21,'label');
		basetime = tic;
		frame = 0;
	endif
until(killflag && ishandle(f))

% ---- Close:

system(['kill -9 ' num2str(sim_pid)]);
