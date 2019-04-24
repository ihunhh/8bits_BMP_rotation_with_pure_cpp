# 8bits gray BMP rotation with pure C++

* For ***8bits gray BMP*** image clockwise rotation between ***0~360*** degree

  * ***Without any CV API***
  
  * Interpolation method based on ***Nearest Neighbor***
  
  * Procedure:
  
    * Generating a ***trigonometric functions*** list for ***saving computing overhead***
  
    * Computing new image size
    
    * Mapping each point in new image back to original image ***(360 - withch the degree you want to ratation)***

  * ***Original image:***
 
     <img src="/woman.bmp" width="368">
     
  * ***Rotation 279 degree:***
  
     <img src="/rotation_1.bmp" width="368">
  


