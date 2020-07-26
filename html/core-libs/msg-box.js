function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

class MessageBox {
	constructor(area_id, template_loc) {
        this.id = area_id;
        this.template_loc = template_loc;
    }	

    create(duration, msg) {
        let container_div = document.createElement("div");
        let msg_div = document.createElement("div");
        let msg_close = document.createElement("a");

        //classes
        container_div.classList.add("msgbox-box");
        msg_div.classList.add("msgbox-content");
        msg_close.classList.add("msgbox-close");

        //text
        msg_div.textContent = msg;
        msg_close.textContent = "Close";

        //callbacks
        msg_close.addEventListener("click", function() {
            container_div.classList.add("fadeOut");
            sleep(500).then(() => { container_div.remove(); });
        })

        container_div.appendChild(msg_div);
        container_div.appendChild(msg_close);


        document.getElementById(this.id).appendChild(container_div);

        setTimeout(function() {
            container_div.classList.add("fadeOut");
            sleep(500).then(() => { container_div.remove(); });
        }, duration);
    }
}