#ifndef ALGORITMOS_H_INCLUDED
#define ALGORITMOS_H_INCLUDED

#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>



#define INT_MAX 10000;


struct point
{
    float x,y;
    bool state;
    point(float a, float b):x(a),y(b){}
    point(){}
};


float euclidean(point u, point v){
    return sqrt(pow(u.x-v.x,2)+pow(u.y-v.y,2));
}


int inicio=0, fin=510; int pt_del=24;
vector<int> ptsDel;
int num_points = 25*25;
vector<point> points(num_points);
vector<int> graph[25*25]; ///matriz



bool BFS(int prev[], int dist[])
{
    list<int> q;

    bool visited[num_points];
    for (int i = 0; i < num_points; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    visited[inicio] = true;
    dist[inicio] = 0;
    q.push_back(inicio);
   while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        for (int i = 0; i < graph[u].size(); i++) { ///adyacents
            if (visited[graph[u][i]] == false && points[graph[u][i]].state == 1) {
                visited[graph[u][i]] = true;
                dist[graph[u][i]] = dist[u] + 1;
                prev[graph[u][i]] = u;
                q.push_back(graph[u][i]);

                ///dibujo
                glLineWidth(1.5);
                glBegin(GL_LINES);
                glColor3f(1.0,0,0);
                glVertex2f(points[u].x, points[u].y);
                glVertex2f(points[graph[u][i]].x, points[graph[u][i]].y);
                glEnd();
                if (graph[u][i] == fin)
                   return true;
            }
        }
    }

    return false;
}


bool DFS(int prev[], int dist[])
{
    stack<int> s;
    bool visited[num_points];
    for (int i = 0; i < num_points; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    visited[inicio] = true;
    dist[inicio] = 0;
    s.push(inicio);

   while (!s.empty()) {
        int u = s.top();
        s.pop();
        for (int i = 0; i < graph[u].size(); i++) {
            if (visited[graph[u][i]] == false && points[graph[u][i]].state == 1) {
                visited[graph[u][i]] = true;
                dist[graph[u][i]] = dist[u] + 1;
                prev[graph[u][i]] = u;
                s.push(graph[u][i]);

                ///dibuj
                if (graph[u][i] == fin)
                   return true;
            }

        }
            glLineWidth(1.5);
            glBegin(GL_LINES);
            glColor3f(1.0,0,0);
            glVertex2f(points[u].x, points[u].y);
           glVertex2f(points[s.top()].x, points[s.top()].y);
            glEnd();

    }

    return false;
}

float opt_euclidean(int u, int v)
{
    float x1 = points[u].x - points[v].x;
    float y1 =  points[u].y - points[v].y;
    return ((x1*x1) + (y1*y1));
}



bool best_first(int prev[], int dist[])
{
    stack<int>q; // Create an empty PriorityQueue
    q.push(inicio); // PriorityQueue pq; Insert "start" in pq.

    bool visited[num_points];
    for (int i = 0; i < num_points; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    visited[inicio] = true;
    dist[inicio] = 0;


     while (!q.empty()) { //3) Until PriorityQueue is empty
        int u = q.top();  // u = PriorityQueue.DeleteMin
        q.pop();
        vector<pair<float, int> >minimos;
        for (int i = 0; i < graph[u].size(); i++) {
                 if (visited[graph[u][i]] == false && points[graph[u][i]].state == 1){
                 visited[graph[u][i]] = true;
                //if(points[graph[u][i]].state)
                    minimos.push_back(make_pair(opt_euclidean(graph[u][i], fin),graph[u][i] ));
                 }
                   // cout<<opt_euclidean(graph[u][i], fin)<<endl ;
                 //}
        }
         sort(minimos.begin(), minimos.end());
        // cout<<"minimo: "<<minimos[0].first<<" "<<minimos[0].second<<endl;
         for(int i = minimos.size()-1; i >= 0; i--){
            q.push(minimos[i].second);
            prev[minimos[i].second]= u;
            dist[minimos[i].second] = dist[u]+1;

             glLineWidth(1.5);
                glBegin(GL_LINES);
                glColor3f(1.0,0,0);
                glVertex2f(points[u].x, points[u].y);
                glVertex2f(points[minimos[i].second].x, points[minimos[i].second].y);
                glEnd();
            if(minimos[i].second  == fin)
            {
           //  cout<<"termino"<<endl;
                return true;
            }
         }


     }
     return false;
}


#endif // ALGORITMOS_H_INCLUDED
