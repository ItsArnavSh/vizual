<script lang="ts">
    import { onMount } from "svelte";
    let canvas: HTMLCanvasElement;
    let ctx: CanvasRenderingContext2D;
    let width: number;
    let height: number;
    let beingDragged: Node | null = null;
    let drag: boolean = false;
    const C1 = 0.1; // Attraction constant
    const C2 = 1000; // Repulsion constant
    const particleRepulsionConst = 5000; // Repulsion constant between particles
    const dampingFactor = 0.989; // Damping factor to slow down the particle over time
    const minSpeed = 0.1; // Minimal speed to keep particles moving
    const maxSpeed = 5; // Maximum speed for particles
    let side: number; // Side will be the dimensions of the square frame
    let NodeList: Node[] = [];
    let cursorX = 0,
        cursorY = 0;

    class Node {
        x: number;
        y: number;
        vx: number;
        vy: number;
        radius: number;
        constructor(x: number, y: number, r: number, vx: number, vy: number) {
            this.x = x;
            this.y = y;
            this.vx = vx;
            this.vy = vy;
            this.radius = r;
        }
    }

    onMount(() => {
        // Setting up the canvas dimensions
        width = window.innerWidth;
        height = window.innerHeight;
        side = width / 2;
        canvas.height = side;
        canvas.width = side;

        // Setting up ctx, and ensuring it is not empty
        if (canvas.getContext("2d") != null) ctx = canvas.getContext("2d")!;
        else throw Error("CTX is empty");

        // Making the background for the first time
        ctx.fillStyle = "#333333";
        ctx.fillRect(0, 0, side, side);

        // Now we spawn some random Nodes
        for (let i = 0; i < 10; i++) {
            NodeList.push(
                new Node(
                    randInt(side / 10, (side * 3) / 4),
                    randInt(side / 10, (side * 3) / 4),
                    20,
                    randInt(0, 1),
                    randInt(0, 1),
                ),
            );
        }
        drawNodes();
    });

    // Utility Functions
    function randInt(min: number, max: number): number {
        return Math.round(Math.random() * (max - min) + min);
    }

    function drawNodes() {
        ctx.fillStyle = "#333333";
        ctx.fillRect(0, 0, side, side);
        ctx.strokeStyle = "#F4FFF8";
        for (let i = 0; i < NodeList.length; i++) {
            ctx.beginPath();
            ctx.arc(
                NodeList[i].x,
                NodeList[i].y,
                NodeList[i].radius,
                0,
                Math.PI * 2,
                true,
            );
            ctx.lineWidth = 5;
            ctx.stroke();
        }
        calculatePhysics();
        requestAnimationFrame(drawNodes);
    }

    function calculatePhysics() {
        for (let i = 0; i < NodeList.length; i++) {
            let distanceX = NodeList[i].x - side / 2;
            let distanceY = NodeList[i].y - side / 2;
            let distanceFromCentersq = distanceX ** 2 + distanceY ** 2;

            // Calculate net force: attraction - repulsion
            let netForce =
                C1 / distanceFromCentersq - C2 / distanceFromCentersq ** 2;

            // Update velocities based on the net force
            NodeList[i].vx +=
                (netForce * -distanceX) / Math.sqrt(distanceFromCentersq);
            NodeList[i].vy +=
                (netForce * -distanceY) / Math.sqrt(distanceFromCentersq);

            // Apply damping to slow down the particle over time
            NodeList[i].vx *= dampingFactor;
            NodeList[i].vy *= dampingFactor;

            // Ensure the particle doesn't stop completely
            if (Math.abs(NodeList[i].vx) < minSpeed)
                NodeList[i].vx = minSpeed * Math.sign(NodeList[i].vx);
            if (Math.abs(NodeList[i].vy) < minSpeed)
                NodeList[i].vy = minSpeed * Math.sign(NodeList[i].vy);

            // Ensure the particle's speed does not exceed maxSpeed
            if (Math.abs(NodeList[i].vx) > maxSpeed)
                NodeList[i].vx = maxSpeed * Math.sign(NodeList[i].vx);
            if (Math.abs(NodeList[i].vy) > maxSpeed)
                NodeList[i].vy = maxSpeed * Math.sign(NodeList[i].vy);

            // Handle boundary collisions
            if (
                NodeList[i].x - NodeList[i].radius < 0 ||
                NodeList[i].x + NodeList[i].radius > side
            ) {
                NodeList[i].vx = -NodeList[i].vx;
            }
            if (
                NodeList[i].y - NodeList[i].radius < 0 ||
                NodeList[i].y + NodeList[i].radius > side
            ) {
                NodeList[i].vy = -NodeList[i].vy;
            }
        }

        // Calculate repulsion between particles
        for (let i = 0; i < NodeList.length; i++) {
            for (let j = i + 1; j < NodeList.length; j++) {
                let dx = NodeList[j].x - NodeList[i].x;
                let dy = NodeList[j].y - NodeList[i].y;
                let distanceSq = dx ** 2 + dy ** 2;
                let distance = Math.sqrt(distanceSq);
                let minDistance = NodeList[i].radius + NodeList[j].radius;

                if (distance < minDistance) {
                    let overlap = minDistance - distance;
                    let force = (particleRepulsionConst * overlap) / distanceSq;

                    let fx = (force * dx) / distance;
                    let fy = (force * dy) / distance;

                    NodeList[i].vx -= fx / 2;
                    NodeList[i].vy -= fy / 2;
                    NodeList[j].vx += fx / 2;
                    NodeList[j].vy += fy / 2;
                }
            }
        }

        // Update positions based on velocities after repulsion calculations
        for (let i = 0; i < NodeList.length; i++) {
            NodeList[i].x += NodeList[i].vx;
            NodeList[i].y += NodeList[i].vy;
        }
    }

    function startedDragging(event: MouseEvent) {
        console.log("Started");
        const rect = canvas.getBoundingClientRect();
        cursorX = event.clientX - rect.left;
        cursorY = event.clientY - rect.top;
        for (let i = 0; i < NodeList.length; i++) {
            if (
                NodeList[i].radius ** 2 >=
                (cursorX - NodeList[i].x) ** 2 + (cursorY - NodeList[i].y) ** 2
            ) {
                beingDragged = NodeList[i];
                drag = true;
                break; // Start dragging the first node found under the cursor
            }
        }
        if (beingDragged == null) {
            return; // This can be later used to drag the view
        }
    }

    function endedDragging(event: MouseEvent) {
        drag = false;
        beingDragged = null;
    }

    function handleMouseMove(event: MouseEvent) {
        const rect = canvas.getBoundingClientRect();
        cursorX = event.clientX - rect.left;
        cursorY = event.clientY - rect.top;
        if (drag && beingDragged != null) {
            beingDragged.x = cursorX;
            beingDragged.y = cursorY;
            beingDragged.vx = 0;
            beingDragged.vy = 0;
        }
    }
</script>

<div>
    <canvas
        class="mainCanvas"
        bind:this={canvas}
        on:mousedown={startedDragging}
        on:mouseup={endedDragging}
        on:mousemove={handleMouseMove}
    ></canvas>
</div>

<style>
    .mainCanvas {
        outline: none;
    }
</style>
