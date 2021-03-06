/**
 *******************************************************************************
 * @file       system.c
 * @version    V0.7.4
 * @date       2020.04.18
 * @brief      Some system function for GUI engine.	
 *******************************************************************************
 */ 

#include <cogui.h>
#include <stdarg.h>         /* for va function */

/**
 *******************************************************************************
 * @brief      First step of GUI engine: initial everything.
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void gui_system_init(void)
{
    gui_server_init();
}

/**
 *******************************************************************************
 * @brief      Memory allocate from CoOS.
 * @param[in]  size     How much memory to allocate.
 * @param[out] None
 * @retval     *ptr     Allocated memory pointer.
 *******************************************************************************
 */
void *gui_malloc(uint32_t size)
{
    void *ptr;
    
    ptr = CoKmalloc(size);

    /* if malloc failed, print error message */
	if(ptr == Co_NULL){
		gui_printf("[System] out of memory\r\n");
	}
	
    return ptr;
}

/**
 *******************************************************************************
 * @brief      Free memory to CoOS.
 * @param[in]  *ptr     Memory pointer to free.
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void gui_free(void* ptr)
{
    CoKfree(ptr);
}

/**
 *******************************************************************************
 * @brief      Ack a event.
 * @param[in]  *event       Which event to ack.
 * @param[in]  status       Which status should ack.
 * @param[out] None       
 * @retval     GUI_E_OK     Always return GUI_E_OK .  
 *******************************************************************************
 */
StatusType gui_ack(event_t *event, StatusType status)
{
    ASSERT(event != Co_NULL);
    ASSERT(event->ack);
	
    /* ACK status */
    CoPostMail(event->ack, (void *)&status);

    return GUI_E_OK;
}

/**
 *******************************************************************************
 * @brief      Send a event from CoOS's mail box system.
 * @param[in]  *app             Which application should send to.
 * @param[in]  *event           Event pointer.
 * @param[out] None
 * @retval     GUI_E_OK         If send successfully.         
 * @retval     GUI_E_ERROR      If some error occurred.          
 *******************************************************************************
 */
StatusType gui_send(app_t *app, event_t *event)
{
    StatusType result;

    ASSERT(event != Co_NULL);
    ASSERT(app != Co_NULL);

    /* send event to application */
    result = CoPostMail(app->mq, event);

    /* return result to GUI status type */
    GUI_RETURN_TYPE(result);
}

/**
 *******************************************************************************
 * @brief      Send a event from CoOS's mail box system, and wait for Ack.
 * @param[in]  *app             Which application should send to.
 * @param[in]  *event           Event pointer.
 * @param[out] None
 * @retval     GUI_E_OK         If send successfully.         
 * @retval     GUI_E_ERROR      If some error occurred.          
 *******************************************************************************
 */
StatusType gui_send_sync(app_t *app, event_t *event)
{
    StatusType result;

    ASSERT(event != Co_NULL);
    ASSERT(app != Co_NULL);

    /* create a mail box for acking */
    OS_EventID mq = CoCreateMbox(EVENT_SORT_TYPE_FIFO);
	event->ack = mq;

    /* send event to application */
    result = CoPostMail(app->mq, event);
    
    /* if send event failed, return */
    if (result != E_OK){
        CoDelMbox(mq, OPT_DEL_ANYWAY);
        GUI_RETURN_TYPE(result);
    }

    /* wait forever for server ack */
    event = CoPendMail(mq, 0, &result);

    /* delete the mail box after receive ack */
    CoDelMbox(mq, OPT_DEL_ANYWAY);

    /* return result to GUI status type */
    GUI_RETURN_TYPE(result);
}

/**
 *******************************************************************************
 * @brief      Receive a event from CoOS's mail box system.
 * @param[in]  mq               Which mail box should receive
 * @param[in]  timeout          How long should it wait
 * @param[out] event            Got event pointer
 * @retval     GUI_E_OK         Receive event successfully
 * @retval     GUI_E_ERROR      Something wrong when receiveing a event
 *******************************************************************************
 */
StatusType gui_recv(OS_EventID mq, event_t *event, int32_t timeout)
{
    StatusType result;
    app_t *app;
    event_t* buf;

    ASSERT(event!=Co_NULL);

    /* check running applicate is vaild or not */
    app = gui_app_self();
    if (app == Co_NULL) {
        return GUI_E_APP_NULL;
    }

    /* receive a event and copy to event pointer */
    buf = (event_t *)CoPendMail(mq, timeout, &result);
    gui_memcpy(event, buf, sizeof(event_t));

    /* return result to GUI status type */
    GUI_RETURN_TYPE(result);
}

/**
 *******************************************************************************
 * @brief      Set memory buffer to a value.
 * @param[in]  *buf             Memory buffer to set.
 * @param[in]  val              Set to this value.
 * @param[in]  size             How much should set.
 * @param[out] *buf             Result after setting.
 * @retval     *buf             Result after setting.
 *******************************************************************************
 */
void *gui_memset(void *buf, int val, uint64_t size)
{
    char *tmp = (char *)buf;

    /* setting value one by one */
    while (size--) {
        *tmp++ = val;
    }

    return buf;
}

/**
 *******************************************************************************
 * @brief      Copy a memory buffer value to another memory buffer.
 * @param[in]  *src             Memory buffer to copy.
 * @param[in]  size             How much should copy.
 * @param[out] *dest            Memory buffer to paste.
 * @retval     *dest            Result after pasting.
 *******************************************************************************
 */
void *gui_memcpy(void *dest, const void *src, uint64_t size)
{
    char *tar = (char *)dest;
    char *tmp = (char *)src;

    /* pasting value one by one */
    while (size--) {
        *tar++ = *tmp++; 
    }

    return dest;    
}

/**
 *******************************************************************************
 * @brief      Moving a memory buffer value to another memory buffer.
 * @param[in]  *src             Memory buffer to move.
 * @param[in]  size             How much should move.
 * @param[out] *dest            Destination of moving.
 * @retval     *dest            Result after moving.
 *******************************************************************************
 */
void *gui_memmove(void *dest, const void *src, uint64_t size)
{
    char *ds = (char *)dest;
    char *ss = (char *)src;

    /* if destination is on the back of source, and the size will not overwrite */
    if (ss < ds && ds < ss + size) {
        ds += size;
        ss += size;

        /* moving from the last slot*/
        while (size--) {
            *(--ds) = *(--ss);
        }
    }
    else {

        /* moving from the first slot */
        while (size--) {
            *ds++ = *ss++;
        }
    }

    return dest;
}

/**
 *******************************************************************************
 * @brief      Compare two memory buffer value.
 * @param[in]  *src             Memory buffer 1.
 * @param[in]  *src             Memory buffer 2.
 * @param[in]  size             How much should compare.
 * @param[out] None
 * @retval     res              Value of first different slot.
 *******************************************************************************
 */
int32_t gui_memcmp(const void *buf1, const void *buf2, uint64_t size)
{
    const unsigned char *s1, *s2;
    int res = 0;

    for (s1 = (const unsigned char *)buf1, s2 = (const unsigned char *)buf2; size > 0; ++s1, ++s2, --size) {

        /* if different is occurred */
        if ((res = *s1 - *s2) != 0) {
            break;
        }
    }
     
    return res;
}

/**
 *******************************************************************************
 * @brief      Find the substring in a string.
 * @param[in]  *src             Main string for find.
 * @param[in]  *tar             Target to find.
 * @param[out] None
 * @retval     res              Where we first find the substring.
 * @retval     Co_NULL          If we found nothing.
 *******************************************************************************
 */
char *cogui_strstr(const char *src, const char *tar)
{
    int ls, lt;

    /* if target is NULL, just return the start point of source */
    lt = gui_strlen(src);
    if (!lt) {
        return (char *)src;
    }

    /* only if target's length is shorten then source's length should search */
    ls = gui_strlen(tar);
    while (ls >= lt) {
        --ls;

        /* if we found one, return occurred point */
        if (!gui_memcmp(src, tar, lt)) {
            return (char *)src;
        }

        ++src;
    }     

    /* if we can't find substring, return Co_NULL */
    return Co_NULL;
}

/**
 *******************************************************************************
 * @brief      Get string length.
 * @param[in]  *str      To get the length of string.
 * @param[out] None
 * @retval     length    Result of string length. 
 *******************************************************************************
 */
uint64_t gui_strnlen(const char *str, uint64_t maxlen)
{
    const char *s;

    /* goto the end of the string */
    for (s = str; *s && maxlen; ++s, --maxlen) {
        /* passing */
    }
    
    /* return memory address's differece */
    return s-str;
}

/**
 *******************************************************************************
 * @brief      Get string length.
 * @param[in]  *str      To get the length of string.
 * @param[out] None
 * @retval     length    Result of string length. 
 *******************************************************************************
 */
uint64_t gui_strlen(const char *str)
{
    const char *s;

    /* goto the end of the string */
    for (s = str; *s != '\0'; ++s) {
        /* passing */
    }
    
    /* return memory address's differece */
    return s - str;
}

/**
 *******************************************************************************
 * @brief      Duplicate a string.
 * @param[in]  *str      Target string to duplicate.
 * @param[out] None
 * @retval     *tmp      Result of new string pointer.
 *******************************************************************************
 */
char *gui_strdup(const char *str)
{   
    /* allocate a memory for duplicate */
    uint64_t len = gui_strlen(str) + 1; /* need to plus one for '/0' */
    char *tmp =  (char *)gui_malloc(len);

    /* if allocate failed, return Co_NULL */
    if (tmp == Co_NULL) {
        return tmp;
    }

    /* using gui_memmove to finish the work */
    gui_memmove(tmp, str, len);

    return tmp;
}

/**
 *******************************************************************************
 * @brief      Compare two strings with specific length
 * @param[in]  *str1      String 1 to compare.
 * @param[in]  *str2      String 2 to compare.
 * @param[in]  len        How long should we compare.
 * @param[out] None
 * @retval     val        Value of two strings difference on first occurred.
 * @retval     0          Return 0 if two strings are equal.
 *******************************************************************************
 */
int32_t gui_strncmp(const char *str1, const char *str2, uint64_t len)
{
    /* if two strings value are different or out of range, will break this loop */
    for (; *str1 && *str1 == *str2 && len; ++str1, ++str2, --len) {
        /* passing */
    }

    /* return differece of two strings value in current slot */
    return (*str1 - *str2);
}

/**
 *******************************************************************************
 * @brief      Compare two strings.
 * @param[in]  *str1      String 1 to compare.
 * @param[in]  *str2      String 2 to compare.
 * @param[out] None
 * @retval     val        Value of two strings difference on first occurred.
 * @retval     0          Return 0 if two strings are equal.
 *******************************************************************************
 */
int32_t gui_strcmp(const char *str1, const char *str2)
{
    /* if two strings value are different, will break this loop */
    for (; *str1 && *str1 == *str2; ++str1, ++str2) {
        /* passing */
    }

    /* return differece of two strings value in current slot */
    return (*str1 - *str2);
}

/**
 *******************************************************************************
 * @brief      Compute the power of x.
 * @param[in]  base     Base number.
 * @param[in]  exp      Exponent number.
 * @param[out] None
 * @retval     sum      Result of x power by y. 
 *******************************************************************************
 */
uint64_t gui_pow(int32_t base, int32_t exp)
{
    uint64_t sum = 1;

    /* multiplicative base */
    while (exp--)
        sum *= base;

    return sum;
}

/**
 *******************************************************************************
 * @brief      Transform integer to string.
 * @param[in]  n        Integer to transform.
 * @param[out] *ss      Transform result.
 * @retval     None
 *******************************************************************************
 */
void gui_itoa(int16_t n, char *ss)
{
    int i, j, sign, k;
    char s[10];

    /* determine the signed */
    if((sign=n)<0)
        n=-n;

    i=0;
    do{
        s[i++]=n%10+'0';
    }while ((n/=10)>0);

    /* add '-' if it is negative */
    if(sign<0)
        s[i++]='-';

    /* add end character to the end of the string */
    s[i]='\0';

    /* reverse the result to real string */
    for(j=i-1, k=0; j>=0; j--, k++) {
        ss[k] = s[j];
    }

    /* add end character to the end of the string */
    ss[k]='\0';
}

/**
 *******************************************************************************
 * @brief      Print message via UART.
 * @param[in]  *str    What message should print.
 * @param[out] None
 * @retval     res     How long we print.
 *******************************************************************************
 */
#ifdef COGUI_DEBUG_PRINT
int gui_printf(const char *str,...)
{
    va_list ap;
    int val,r_val,space=0;
	char count, ch;
	char *s = Co_NULL;
    int res = 0;

    //CoSchedLock();
    va_start(ap,str);
    while ('\0' != *str) { 
          switch (*str)
          {
            case '%':
                str++;
                while (*str >= '0' && *str <= '9') {
                    space *= 10;
                    space += *str - '0';
                    str++;
                }

                switch (*str) {

                    /* handle integer var */
                    case 'd':
                        val = va_arg(ap, int); 
 			            
                        /* if val is negative or zero */
                        if (val < 0) {
                            cogui_putchar('-');
                            val = 0 - val;
                        }
                        else if (val == 0) {
                            cogui_putchar('0');
                        }
                        
                        r_val = val; 
                        count = 0; 
						while (r_val) {
                            count++;
                            r_val /= 10;
                        }
						res += count;
                        r_val = val; 
                        while (count) { 
                            ch = r_val / gui_pow(10,count - 1);
							r_val %= gui_pow(10, count - 1);
							cogui_putchar(ch + '0');
							count--;
						}
                        break;

                    /* handle integer var with hex output */
                    case 'x':
                        val = va_arg(ap, int);

                        /* if val is negative or zero */
                        if (val<0) {
                            cogui_putchar('-');
                            val = 0 - val;
                        }
                        else if (val == 0) {
                            cogui_putchar('0');
                        }
                        
                        r_val = val; 
                        count = 0;
						while (r_val) {
                            count++;
                            r_val /= 16; 
                        }
                        res += count;
                        r_val = val; 
                        while(count) { 
                            ch = r_val / gui_pow(16, count - 1);
							r_val %= gui_pow(16, count - 1);
							if (ch <= 9)
                                cogui_putchar(ch + '0');
                            else
								cogui_putchar(ch - 10 + 'a');
							count--;
						}
						break;

                    /* handle string var */
                    case 's':
						s = va_arg(ap, char *);
                        
                        if (space) {
                            int len = gui_strlen(s);
                            while (len < space) {
                                cogui_putchar(' ');
                                space --;
                            }
                        }

						cogui_putstr(s);
                        res += gui_strlen(s);
						break;
					
                    /* handle character var */
                    case 'c':
                        cogui_putchar( (char)va_arg(ap, int ));
						res += 1;
                        break;

                    default:
						break;
				}
				break;
            
            /* handle escape character: newline */
            case '\n':
				cogui_putchar('\n');
				res += 1;
				break;
			
            /* handle escape character: return */
			case '\r':
				cogui_putchar('\r');
				res += 1;
				break;
			
            /* just output character */
			default:
				cogui_putchar(*str);
				res += 1;
		}
		str++;
     }
    va_end(ap);
	//CoSchedUnlock();

	return res;
}
#else
int gui_printf(const char *str,...){
    /* do nothing */
}
#endif /* COGUI_DEBUG_PRINT */

/**
 *******************************************************************************
 * @brief      Print assert failed message and panic.
 * @param[in]  *ex_string    What assert failed.
 * @param[in]  *func         Assert failed in which function.
 * @param[in]  line          Detailing in which line assert failed.
 * @param[out] None
 * @retval     None
 *******************************************************************************
 */
void assert_handler(const char *ex_string, const char *func, uint32_t line)
{
    /* output message to screen */
    gui_assert_failed_page(ex_string, line, func);

	volatile char dummy = 0;

    /* print error message */
	gui_printf("(%s) assertion failed at function: %s, line number: %d.\r\n", ex_string, func, line);

    /* loop forever */
	while(dummy == 0)
	{
        /* panic */
	}
}
