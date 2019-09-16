#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 50
#define L 1.0
#define R 0.028
#define dt 0.002

typedef struct {
  double x, y, vx, vy;
} particle;

void init(particle *p){
  int i, j;
  int condition=0;
  double dist, mindist;

  for(i=0; i<N; i++){
    p[i].vx = (double)rand()/RAND_MAX*2.-1.;
    p[i].vy = (double)rand()/RAND_MAX*2.-1.;
  }
  
  // enquanto todas as partículas não estiverem sem overlap
  while( condition == 0 ){
    // sorteia a posição da primeira partícula
    p[0].x = (double)rand()/RAND_MAX*(L-2*R)+R;
    p[0].y = (double)rand()/RAND_MAX*(L-2*R)+R;
    for(i=1; i<N; i++){
      // sorteia a posição de uma partícula
      p[i].x = (double)rand()/RAND_MAX*(L-2*R)+R;
      p[i].y = (double)rand()/RAND_MAX*(L-2*R)+R;

      // mede a distancia min até uma partícula já escolhida
      mindist = 100;
      for(j=0; j<i; j++){
  	dist = sqrt(pow(p[i].x-p[j].x, 2)+pow(p[i].y-p[j].y, 2));
  	if (mindist > dist) {
  	  mindist = dist;
  	}
      }

      // se a dist min até uma part for < 2R -> RECOMEÇAR
      if (mindist < 2.*R) {
  	condition = 0;
  	break;
      }
      else {
  	condition = 1;
      }
    }
  }
}

double next_wall(particle *p, int *disk, int *dir){
  int i;
  double delt, dtmin=1000.;
  
  // tempo min para colisão na parede
  for(i=0; i<N; i++){
    // direção x e indo para direita
    if(p[i].vx > 0){
      delt = (L-R-p[i].x)/p[i].vx;
      if(dtmin > delt){
	dtmin = delt;
	*disk = i;
	*dir = 0;
      }
    }
    // direção x e indo para esquerda
    else if(p[i].vx < 0){
      delt = (p[i].x-R)/(-p[i].vx);
      if(dtmin > delt){
	dtmin = delt;
	*disk = i;
	*dir = 0;
      }
    }
    
    // direção y e indo para direita
    if(p[i].vy > 0){
      delt = (L-R-p[i].y)/p[i].vy;
      if(dtmin > delt){
	dtmin = delt;
	*disk = i;
	*dir = 1;
      }
    }
    // direção y e indo para esquerda
    else if(p[i].vy < 0){
      delt = (p[i].y-R)/fabs(p[i].vy);
      if(dtmin > delt){
	dtmin = delt;
	*disk = i;
	*dir = 1;
      }
    }
  }

  return dtmin;
}
double next_pair(particle *p, int *disk1, int *disk2){
  int i, j;
  double d, dx, dy, dv, dvx, dvy;
  double scal, ups, delt, dtmin=1000;
  
  for(i=0; i<N; i++){
    for(j=0; j<i; j++){
      dx = p[i].x-p[j].x;
      dy = p[i].y-p[j].y;
      d  = dx*dx+dy*dy;
      
      dvx = p[i].vx-p[j].vx;
      dvy = p[i].vy-p[j].vy;
      dv  = dvx*dvx+dvy*dvy;

      scal = dvx*dx+dvy*dy;
      ups  = scal*scal-dv*(d-4.*R*R);
      if(ups>0 && scal<0){
	delt = -(scal+sqrt(ups))/dv;
	if(dtmin > delt){
	  dtmin = delt;
	  *disk1 = i;
	  *disk2 = j;
	}
      }
    }
  }

  return dtmin;
}

int main(int argc, char *argv[]){
  int i, ev, col_disk, col_disk1, col_disk2, col_dir;
  double dtwall, dtpair, t, next_event, next_t, remain_t;
  double d, dx, dy, dv, dvx, dvy;
  particle p[N];

  srand(time(0));
  init(p);

  dtwall = next_wall(p, &col_disk, &col_dir);
  dtpair = next_pair(p, &col_disk1, &col_disk2);
  (dtwall < dtpair) ? (next_event = dtwall) : (next_event = dtpair);
  while(t<100){
    next_t = t+dt;
    // se o próximo evento for no próximo passo dt
    while(t+next_event <= next_t){
      t += next_event;

      // atualiza as posições conforme com o dt do proximo evento
      for(i=0; i<N; i++){
	p[i].x += p[i].vx*next_event;
	p[i].y += p[i].vy*next_event;
      }

      // atualiza as velocidades
      if(dtwall < dtpair){
	if(col_dir == 0)
	  p[col_disk].vx*=-1;
	else
	  p[col_disk].vy*=-1;
      }
      else{
	dx = p[col_disk1].x-p[col_disk2].x;
	dy = p[col_disk1].y-p[col_disk2].y;
	d  = sqrt(dx*dx+dy*dy);
      
	dvx = p[col_disk1].vx-p[col_disk2].vx;
	dvy = p[col_disk1].vy-p[col_disk2].vy;
	dv  = sqrt(dvx*dvx+dvy*dvy);

	p[col_disk1].vx -= dx/d*(dvx*dx+dvy*dy)/d;
	p[col_disk1].vy -= dy/d*(dvx*dx+dvy*dy)/d;
	p[col_disk2].vx += dx/d*(dvx*dx+dvy*dy)/d;
	p[col_disk2].vy += dy/d*(dvx*dx+dvy*dy)/d;
      }

      // checa as próximas colisões, tanto de partícula-muro quanto partícula-partícula
      dtwall = next_wall(p, &col_disk, &col_dir);
      dtpair = next_pair(p, &col_disk1, &col_disk2);
      (dtwall < dtpair) ? (next_event = dtwall) : (next_event = dtpair);
    }

    remain_t = next_t-t;
    for(i=0; i<N; i++){
      p[i].x += p[i].vx*remain_t;
      p[i].y += p[i].vy*remain_t;
    }
    t += remain_t;
    next_event -= remain_t;

    // imprime
    for(i=0; i<N; i++){
      printf("%lf %lf %lf\n", p[i].x, p[i].y, R*1000);
    }
  }
  
  return 0;
}
