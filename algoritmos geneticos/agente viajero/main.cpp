#include "grafo.h"

int main(int argc, char** argv) {

	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	
	ALLEGRO_DISPLAY* ventana = al_create_display(600, 650);
	al_set_window_title(ventana, "Agente Viajero");
	
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());	
	
	ALLEGRO_EVENT event;
	
	int pos_x, pos_y;	//posicion del mouse
	bool agregar_nodos = true;
	
	TSP tsp;
	while(true) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_wait_for_event(event_queue, &event);
		
		if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if ( (event.mouse.button & 1) && agregar_nodos )
			{
				pos_x = event.mouse.x;
				pos_y = event.mouse.y;
				
				tsp.agregar_ciudad( make_pair(pos_x, pos_y) );

				
			}
		}
		
		if(event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if ( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE ) break;
			
			else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				agregar_nodos = false;
				tsp.algoritmo_genetico();
				
			}
		}
		
		tsp.dibujar_grafo();
		if(agregar_nodos == false)
		{
			
			tsp.dibujar_mejor_camino();
		}
		
		al_flip_display();
	}
	
	
	al_destroy_display(ventana);
    
	return 0;
}