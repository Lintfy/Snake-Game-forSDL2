#include "sdlsys.cpp"
#include <queue>
#include <vector>
#include <random>

using namespace std;

SDL_Surface* gmain=NULL;
random_device rnd;

const int
	dotSize=30, // ドットの大きさ
    stageWH=21, // ステージの縦横の大きさ(必ず奇数!)
    windowWH=dotSize*stageWH,
    stageArea=stageWH*stageWH,
    arrows[]={0,-stageWH,stageWH,1,-1},
	esaSpawn=12;
;

bool stagef[stageArea];


void keyevent(short &flag,int &to,bool &wait,int lng,bool &tlb,bool &rest){

	if(flag>=273 and flag<=276){
		if((flag+1)/2!=(to+273)/2 or lng==1)to=flag-272;
		wait=false;
	}
	else if(flag==303 or flag==304)tlb=true;
	else if(flag==113){
		rest=true;
	}

	flag=0;
}

void updateTitle(int hisc,int sc){
	char title[500];
	sprintf(title,"[ SNAKE ]   HighScore : %d  | Score : %d",hisc-1,sc-1);
	SDL_WM_SetCaption(title,NULL);
	
}

int xpos(int nm){return nm%stageWH;}
int ypos(int nm){return nm/stageWH;}
int topos(int nmx,int nmy){return nmx+nmy*stageWH;}

int mht(int nma,int nmb){
	int nmax=max(xpos(nma),xpos(nmb)),nmay=max(ypos(nma),ypos(nmb)),nmbx=min(xpos(nma),xpos(nmb)),nmby=min(ypos(nma),ypos(nmb));
	return min(nmax-nmbx,nmbx-nmax+stageWH)+min(nmay-nmby,nmby-nmay+stageWH);

}

void setEsa(queue<int> qu,int &pl){
	int mn,sz=0,amn=-1;
	int areas[stageArea];
	vector<int> vc={0};

	while(!qu.empty()){
		areas[sz]=qu.front();
		sz++;
		qu.pop();
	}

	for(int i=0;i<stageArea;i++){
		if(!stagef[i]){
			mn=min(mht(areas[0],i),esaSpawn);
			for(int j=1;j<sz;j++){
				mn=min(min(mn,mht(areas[j],i)),esaSpawn);
			}
		
		
			if(amn!=-1){
				if(mn==amn){
					vc.push_back(i);
				}
				else if(mn>amn){
					amn=mn;
					vc={i};
				}
			}
			else{
				amn=mn;
			}
		}
	}
	pl=vc[rnd()%vc.size()];

}

void reset(int &npos,int &lng,int &to,bool &wait,queue<int> &qu,bool &gov,bool &rest,int &es,int hisc){
	npos=stageArea/2;
	lng=1;
	to=0;
	wait=true;
	gov=false;
	rest=false;
	while(!qu.empty())qu.pop();
	qu.push(npos);

    for(int i=0;i<stageArea;i++){
        stagef[i]=i==npos;
    }
	setEsa(qu,es);
	updateTitle(hisc,lng);
}


void display(int pl,int mt,bool tc,bool gov,int npon,int to){
	auto rectIn= [](int a,int x,int y,int w,int h,int r,int g,int b){rectangle(gmain,x+xpos(a)*dotSize,y+ypos(a)*dotSize,w,h,SDL_MapRGB(gmain->format,r,g,b));};
	const int ds6=dotSize/6,ds8=dotSize/8;
    for(int i=0;i<stageArea;i++){
		
		if(i==pl)rectIn(i,0,0,dotSize,dotSize,255,100+tc*30,100+tc*30);
		if(!(gov and tc)){
			if(i==mt)rectIn(i,0,0,dotSize,dotSize,130,130,130);
			else if(stagef[i])rectIn(i,0,0,dotSize,dotSize,100,100,100);

			if(i==npon and rnd()%20!=0){
				if(gov)rectIn(i,0,0,dotSize,dotSize,80,80,80);
				if(to==1 or to==4)rectIn(i,ds8,ds8,ds6,ds6,255,255,255);
				if(to==1 or to==3)rectIn(i,ds8*7,ds8,ds6,ds6,255,255,255);
				if(to==2 or to==3)rectIn(i,ds8*7,ds8*7,ds6,ds6,255,255,255);
				if(to==2 or to==4)rectIn(i,ds8,ds8*7,ds6,ds6,255,255,255);
			}
		}

    }
}



void movein(int &npos,int to,queue<int> &qu,int &lng,int &pl,bool &gov,int &hisc){

	if(gov)return;

    int ny=ypos(npos);

    npos+=arrows[to];
	
    if(to>=3)npos+=((ypos(npos)<ny or npos<0)-(ypos(npos)>ny))*stageWH;
	else npos+=((npos<0)-(npos>=stageArea))*stageArea;



    if(npos==pl){
		setEsa(qu,pl);
		lng++;
		hisc=max(hisc,lng);
		updateTitle(hisc,lng);
	}
	else{
		stagef[qu.front()]=false;
		qu.pop();
	}
	if(stagef[npos]){
		gov=true;
	}
	stagef[npos]=true;
    qu.push(npos);
	
}

int main(){

	SDL_Init(SDL_INIT_VIDEO);
	
	gmain=SDL_SetVideoMode(windowWH,windowWH,32,SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_Event ev;
	bool
		exitb=false,
		waiting,
		tick=false,
		turbo=false,
		gameOver,
		res
	;
	short keyf;
    int
        np,
        leng,
        into,
		esa,
		hiScore=1
    ;

    queue<int> quin;
    

    reset(np,leng,into,waiting,quin,gameOver,res,esa,hiScore);
	
	
	//int xh=stageWH/2*dotSize,yh=xh;
	while(!exitb){
		SDL_FillRect(gmain, NULL, SDL_MapRGB(gmain->format,255,255,255));
        
		
		while(SDL_PollEvent(&ev)){
			switch(ev.type){
			case SDL_QUIT:
				exitb = true;
				break;
			case SDL_KEYDOWN:
				keyf=short(ev.key.keysym.sym);
				break;
			case SDL_KEYUP:
				if(short(ev.key.keysym.sym)==303 or short(ev.key.keysym.sym)==304)turbo=false;
				break;
			}
			
		}
		tick=!tick;
		
		if(keyf)keyevent(keyf,into,waiting,leng,turbo,res);

		if(!waiting)movein(np,into,quin,leng,esa,gameOver,hiScore);

		if(res)reset(np,leng,into,waiting,quin,gameOver,res,esa,hiScore);

        display(esa,leng==1 ? -1:quin.front(),tick,gameOver,np,into);

		SDL_Flip(gmain);
		SDL_Delay(turbo ? 50:125);
	}
	SDL_Quit();

	return 0;
}
