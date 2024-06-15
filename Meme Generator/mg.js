let generatebtn = document.querySelector(".meme-generator .button-meme-generator-btn")
let memetitle = document.querySelector(".meme-generator .meme-title")
let memeimg = document.querySelector(".meme-generator img")
let memeauthor = document.querySelector(".meme-generator .meme-author")

var updateDetails = (url,author,title) => {
    memeimg.setAttribute("src",url)
    memeauthor.innerHTML = author
    memetitle.innerHTML = title
}

var generateMeme = () => {
    fetch("https://meme-api.com/gimme/wholesomememes")
    .then((response) => response.json())
    .then(data => {
            updateDetails(data.url,data.author,data.title)
        } )
    
}

generatebtn.addEventListener("click",generateMeme)

