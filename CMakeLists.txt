idf_component_register(SRCS "app_socket.c" "app_socket_udp.c" "app_socket_log.c" "app_socket_stdio.c"  "app_socket_uart.c" 
                    INCLUDE_DIRS "." 
                    REQUIRES    "drv_socket"
                                "drv_uart"
                                "drv_rfc2217"
                                    )
                 

