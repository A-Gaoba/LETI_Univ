import wikipedia                                                                        
import help_wiki_function                                               

def find_max_title(lst):
    maximum = 0
    for current_page in lst:                                                      
                                                                     
      if len(wikipedia.page(current_page).summary.split()) >= maximum:
                                                           
         maximum = len(wikipedia.page(current_page).summary.split()) 
         title = wikipedia.page(current_page).title
                                                                               
    return(maximum, title)
  
def create_chain(pages):
    answer = []                                                                     

    for i in range(len(pages)-1): 
                                               
      answer.append(pages[i])                                                       
                                                                                
                                                          
      if pages[i+1] not in wikipedia.page(pages[i]).links:                          
                                                                                
                                                                                
                                                                                
        for checked_outer_link in wikipedia.page(pages[i]).links:                    
          if help_wiki_function.is_page_valid(checked_outer_link):                   
            if pages[i+1] in wikipedia.page(checked_outer_link).links:              
                                                                                
              answer.append(checked_outer_link)                                     
                                                                                
              break                                                                 
                                                                                
                                                                                
    answer.append(pages[-1])  
    return(answer)
    
                                                
pages = input().split(', ')                                                     

if pages[-1] in wikipedia.languages():   
                                                            
                                                                         

  wikipedia.set_lang(pages[-1]) 
  pages.pop()                                                                     
  print(find_max_title(pages)[0], find_max_title(pages)[1])                                                                                          
  print(create_chain(pages)) 
  
else:                                                                                
  print("no results")
