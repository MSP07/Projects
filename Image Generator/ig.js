const accesskey = "40dMe__sj2d_cchx1JI5oXOVFYM9I21pHYOOSPRi1ag";
const form = document.querySelector("form");
const search = document.getElementById("Search-input");
const results = document.querySelector(".Search-results");
const showMore = document.getElementById("show-more-butn");

let input = "";
let page = 1;

async function searchImage() {
    input = search.value;
    const url = `https://api.unsplash.com/search/photos?page=${page}&query=${input}&client_id=${accesskey}`;
    const response = await fetch(url);
    const data = await response.json();
    const resultsData = data.results;

    if (page === 1) {
        results.innerHTML = "";
    }

    resultsData.forEach(result => {
        const imageWrapper = document.createElement("div");
        imageWrapper.classList.add("Search-result");
        const image = document.createElement("img");
        image.src = result.urls.small;
        image.alt = result.alt_description;
        const imageLink = document.createElement("a");
        imageLink.href = result.links.html;
        imageLink.target = "_blank";
        imageLink.textContent = result.alt_description;

        imageWrapper.appendChild(image);
        imageWrapper.appendChild(imageLink);
        results.appendChild(imageWrapper);
    });

    page++;
    if (page > 1) {
        showMore.style.display = "block";
    }
}

form.addEventListener("submit", (event) => {
    event.preventDefault();
    page = 1;
    searchImage();
});

showMore.addEventListener("click", (event) => {
    searchImage();
});
