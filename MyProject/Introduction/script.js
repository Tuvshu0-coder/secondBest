// Toggle sidebar with button
document.getElementById("toggleSideBarBtn").addEventListener("click", function () {
  document.querySelector(".sidebar").classList.toggle("hidden");
});

// Back to top button
document.getElementById("backToTopBtn").addEventListener("click", function () {
  window.scrollTo({ top: 0, behavior: "smooth" });
});

// Keyboard shortcut press "S" to toggle sidebar
document.addEventListener("keydown", function (event) {
  if (event.key === "s" || event.key === "S") {
    document.querySelector(".sidebar").classList.toggle("hidden");
  }
});
