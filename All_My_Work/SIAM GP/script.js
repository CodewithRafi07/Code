document.addEventListener("DOMContentLoaded", () => {
    
    // --- INTRO SCROLL REVEAL ---
    const faders = document.querySelectorAll('.fade-in');
    const appearOptions = {
        threshold: 0.1,
        rootMargin: "0px 0px -30px 0px"
    };

    const appearOnScroll = new IntersectionObserver(function(entries, observer) {
        entries.forEach(entry => {
            if (!entry.isIntersecting) return;
            entry.target.classList.add('appear');
            observer.unobserve(entry.target);
        });
    }, appearOptions);

    faders.forEach(fader => appearOnScroll.observe(fader));

    // --- MOBILE NAVIGATION TOGGLE ---
    const menuToggle = document.getElementById('mobile-menu');
    const navLinks = document.querySelector('.nav-links');

    if (menuToggle) {
        menuToggle.addEventListener('click', () => {
            menuToggle.classList.toggle('active');
            navLinks.classList.toggle('active');
        });
    }

    // Close menu when link clicked
    document.querySelectorAll('.nav-links a').forEach(link => {
        link.addEventListener('click', () => {
            menuToggle.classList.remove('active');
            navLinks.classList.remove('active');
        });
    });

    // --- DESKTOP CUSTOM CURSOR ---
    const cursor = document.querySelector('.cursor');
    if (cursor) {
        document.addEventListener('mousemove', (e) => {
            cursor.style.left = e.clientX + 'px';
            cursor.style.top = e.clientY + 'px';
        });

        const interactiveElements = document.querySelectorAll('a, button, .menu-toggle, .social-links a');
        interactiveElements.forEach(el => {
            el.addEventListener('mouseenter', () => cursor.classList.add('hover-active'));
            el.addEventListener('mouseleave', () => cursor.classList.remove('hover-active'));
        });
    }

    // --- SHOWREEL LIGHTBOX MODAL ---
    const modal = document.getElementById('videoModal');
    const openBtn = document.getElementById('open-showreel');
    const closeBtn = document.querySelector('.close-modal');
    const iframe = document.getElementById('showreel-video');

    if (openBtn && modal) {
        openBtn.addEventListener('click', () => {
            modal.classList.add('open');
            iframe.src = iframe.src.replace("&autoplay=0", "") + "&autoplay=1";
        });

        closeBtn.addEventListener('click', () => {
            modal.classList.remove('open');
            iframe.src = iframe.src; // Stop video
        });

        modal.addEventListener('click', (e) => {
            if (e.target === modal) {
                modal.classList.remove('open');
                iframe.src = iframe.src;
            }
        });
    }

    // --- CONNECT FORM TO EXCEL / GOOGLE SHEET ---
    const contactForm = document.getElementById('client-contact-form');
    const statusMsg = document.getElementById('form-status');

    if (contactForm) {
        contactForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            const submitBtn = document.getElementById('submit-btn');
            submitBtn.innerText = "SENDING...";
            submitBtn.disabled = true;

            const name = document.getElementById('client-name').value;
            const email = document.getElementById('client-email').value;
            const details = document.getElementById('project-details').value;

            const formData = {
                name: name,
                email: email,
                details: details,
                date: new Date().toLocaleString()
            };

            // SheetDB বা FormSubmit-এর API লিংক এখানে দিন
            const API_ENDPOINT = "https://sheetdb.io/api/v1/YOUR_YOUR_ENDPOINT_ID"; 

            if (API_ENDPOINT.includes("YOUR_YOUR_ENDPOINT_ID")) {
                setTimeout(() => {
                    statusMsg.style.color = "#00ff00";
                    statusMsg.innerText = "SUCCESS! DATA SAVED TO LOCAL DATA RUN (CONNECT API IN SCRIPT.JS FOR LIVE SPREADSHEET).";
                    submitBtn.innerText = "SEND TO SPREADSHEET";
                    submitBtn.disabled = false;
                    contactForm.reset();
                }, 1500);
                return;
            }

            fetch(API_ENDPOINT, {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ data: [formData] })
            })
            .then(response => response.json())
            .then(data => {
                statusMsg.style.color = "#00ff00";
                statusMsg.innerText = "SUCCESS! YOUR DATA HAS BEEN EXPORTED TO THE EXCEL SHEET.";
                submitBtn.innerText = "SEND TO SPREADSHEET";
                submitBtn.disabled = false;
                contactForm.reset();
            })
            .catch(error => {
                statusMsg.style.color = "#ff0000";
                statusMsg.innerText = "ERROR CONNECTING TO SPREADSHEET.";
                submitBtn.innerText = "TRY AGAIN";
                submitBtn.disabled = false;
                console.error("Error:", error);
            });
        });
    }
});