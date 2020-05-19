

function getRandomImage() {

    var imgDir = '../images/';
    var imgArray = ['TRAPPIST-1b.png','TRAPPIST-1c.png','TRAPPIST-1d.png','TRAPPIST-1e.png','TRAPPIST-1f.png', 'TRAPPIST-1g.png', 'TRAPPIST-1h.png'];
	var index = 0;
	if (imgArray.length > 1) {
		index = Math.floor(Math.random() * imgArray.length);
		

	var imgPath = imgDir + imgArray[index];

	document.getElementById("backimage").src=imgPath;
	document.getElementById("p_name").innerHTML = imgArray[index].substr(0,11);


	}
	else {
		return false;
	}

}

// Once document is opened / webpage loaded:
// This is jquery

$(document).ready(function(){
	getRandomImage();
  });


			
			function randomImage() {
				var theImage = document.getElementById('myimage');
				var imgDir = 'images/';
				var imgArray = new Array('bigtv.jpg','caddy.jpg','fatcat.jpg','manhart.jpg','roastdog.jpg');
				var imgIndex = 0;
				
				if(imgArray.length > 1) {
					while(imgIndex == lastIndex) {
						imgIndex = Math.floor(Math.random() * imgArray.length);
					}
					lastIndex = imgIndex;
				
					var imgPath = imgDir + imgArray[imgIndex];
					
					theImage.src = imgPath;
					theImage.alt = imgArray[imgIndex];
				}
				else {
					return false;
				}
			}