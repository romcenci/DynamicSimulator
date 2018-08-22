#!/usr/bin/octave -qf

% ------------ Variables:

global fps = 24;
buffsize = 100;
prebuffperc = 24;
global superspeedval = 10;

% ------------ FIFO:

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

% ------------ Initial:

graphics_toolkit('qt');

frame = 0;
global fcount = 0;
rawdata = -1;
prebuff = 0;
olddata = [];
global killflag = 0;
global pauseflag = 1;
global speedflag = 1;
global l;

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

% ------------ Figure:

function r = ternary (expr, true_val, false_val)
	if (expr)
		r = true_val;
	else
		r = false_val;
	endif
endfunction

function playpause(h)
	uiresume();
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
	uiresume();
	global ctrlh outh sim_pid l h fcount;
	set(h,'cdata',ones(l));
	pause(0.001);
	system(['kill -9 ' num2str(sim_pid)]);
	clearfifo();
	fcount = 0;
	[ctrlh,outh,sim_pid] = popen2(argv(){end});
endfunction

function superspeed()
	uiresume();
	global speedflag superspeedval;
	if(speedflag == 1)
		speedflag = superspeedval;
	else
		speedflag = 1;
	endif
endfunction

function setspeed()
	global superspeedval;
	ssvalstring = inputdlg('Set Superspeed (updates per frame, default: 10):','Simulation Options');
	try
		superspeedval = str2double(ssvalstring{1});
	catch
		superspeedval = 10;
	end_try_catch
endfunction

function setfps()
	global fps;
	fpsstring = inputdlg('Set FPS: (default: 24)','Simulation Options');
	try
		fps = str2double(fpsstring{1});
	catch
		fps = 24;
	end_try_catch
endfunction

f = figure('menubar','none','toolbar','figure','units','pixel','name','DynamicSimulator',...
	'numbertitle','off','closerequestfcn','uiresume();killflag=1;','visible','off');

f1 = uimenu ('label', '&File','accelerator', 'f');
f11 = uimenu (f1, 'label', 'Autoscale', 'accelerator', 'a', ...
           'callback', 'global l;axis([0,l(1),0,l(2)])');
f11 = uimenu (f1, 'label', 'Close', 'accelerator', 'q', ...
           'callback', 'global killflag;uiresume();killflag=1;');

f2 = uimenu ('label', '&Simulation','accelerator', 'a');
global f21 = uimenu (f2, 'label', 'Start', 'accelerator', 's',...
	'callback', 'global f21;playpause(f21);');
f22 = uimenu (f2, 'label', 'Superspeed', 'accelerator', 'v', ...
           'callback', 'uiresume();superspeed();');
f23 = uimenu (f2, 'label', 'Reset', 'accelerator', 'r', ...
           'callback', 'uiresume();resetsim();');

f3 = uimenu ('label', '&Options', 'accelerator', 's');
f31 = uimenu (f3, 'label', 'Set FPS','callback', 'setfps()');
f32 = uimenu (f3, 'label', 'Set Superspeed','callback', 'setspeed()');

h = imshow(ones(l));
set(get(h,'parent'),'box','on','boxstyle','full');

t1 = annotation('textbox',[0.02,0.02,0,0],'units','pixels',...
	'verticalalignment','bottom','horizontalalignment','left','linestyle','none');

set(t1,'string',[...
		ternary(pauseflag==0,'running','paused')  "\n"...
		ternary(speedflag==1,'normal','superspeed') "\n"...
		'frame = 0' "\n" ...
		'fps = 0' "\n"...
		'buffer = 0' "\%\n"...
		'l = ' num2str(l) ]);

set(f,'visible','on');

pause(0.001);

% ------------ Main loop:

playpause(f21);

fpsmean = 20;
mfps = zeros(1,fpsmean);
fpstime = tic;
basetime = tic;

do
	if((fsize == 0) && (prebuff == 0)) % need prebuffer?
		prebuff = round(prebuffperc*buffsize/100);
		title buffering;
		pause(0.0001);

	elseif( ( (toc(basetime)*fps) > frame) && (prebuff == 0) && !pauseflag) % plot data if available and not prebuffering and not paused
		frame = frame + 1;
		fcount = fcount + 1;

		% -- plotting:
		set(h,'cdata',pull());
		pause(1e-5);

		mfps = [mfps(2:end) 1/toc(fpstime)];
		fpstime = tic;

		if( (toc(basetime)*fps) > frame)
			basetime = tic;
			frame = 0;
		endif
	endif

	if(isequal(rawdata, -1)) % get data
		fprintf(ctrlh,'p'); fflush(ctrlh);
		rawdata = fgetl(outh);
		fclear(outh);

	elseif(length(rawdata) < l(1)*l(2)) % clean bad data
		rawdata = -1;

	elseif((length(rawdata) >= l(1)*l(2)) && (fsize < buffsize-1)) % add data to buffer
		for i = 1:speedflag
			fprintf(ctrlh,'u');
		endfor
		fflush(ctrlh);

		try
			rawdata = reshape(rawdata == '+',l);
		catch
			disp('Bad data received!');
			rawdata = olddata;
		end_try_catch

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
				title('');
			endif
		endif
	endif

	if(pauseflag)
		basetime = tic;
		frame = 0;
		pause(0.001);
	endif

	set(t1,'string',[...
		ternary(pauseflag==0,'running','paused') "\n"...
		ternary(speedflag==1,'normal','superspeed') "\n"...
		'frame = ' num2str(fcount) "\n"...
		'fps = '  num2str(round(100*mean(mfps))/100) "\n"...
		'buffer = ' num2str(round(100*fsize/buffsize)) "\%\n"...
		'l = ' num2str(l)]);
until(killflag && ishandle(f))

% ------------ Close:

system(['kill -9 ' num2str(sim_pid)]);
