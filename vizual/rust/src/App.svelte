<script lang="ts">
    import { onMount } from "svelte";
    let canvas: HTMLCanvasElement;
    let ctx: CanvasRenderingContext2D;
    let width: number;
    let height: number;
    let beingDragged: Node | null = null;
    let CF1 = 1,
        CF2 = 2;
    let drag: boolean = false;
    const C1 = 100; // Attraction constant
    const C2 = 100000; // Repulsion constant
    const particleRepulsionConst = 5000; // Repulsion constant between particles
    const dampingFactor = 0.9; // Damping factor to slow down the particle over time
    const minSpeed = 0; // Minimal speed to keep particles moving
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
        pointTo: Node[];
        pointToLen: number[];
        pointConnect: boolean[];
        pointShow: boolean[];
        radius: number;
        constructor(
            x: number,
            y: number,
            r: number,
            vx: number = 0,
            vy: number = 0,
        ) {
            this.x = x;
            this.y = y;
            this.vx = vx;
            this.vy = vy;
            this.radius = r;
            this.pointTo = [];
            this.pointToLen = [];
            this.pointConnect = [];
            this.pointShow=[];
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

        createNode(40); //0
        createNode(40); //1
        createNode(40); //2
        createNode(40); //3
        createNode(40); //4
        createNode(40); //5
        createNode(40); //6
        createNode(40); //7
        insertNode(NodeList[0], NodeList[1], 0,false);
        insertNode(NodeList[1], NodeList[2], 0,false);
        insertNode(NodeList[2], NodeList[3], 0,false);
        insertNode(NodeList[3], NodeList[4], 0,false);
        insertNode(NodeList[4], NodeList[5], 0,false);
        insertNode(NodeList[5], NodeList[6], 0,false);
        insertNode(NodeList[6], NodeList[7], 0,false);

        createNode(40); //0
        createNode(40); //1
        createNode(40); //2
        createNode(40); //3
        createNode(40); //4
        createNode(40); //5
        createNode(40); //6
        createNode(40); //7
        insertNode(NodeList[8], NodeList[9]);
        insertNode(NodeList[9], NodeList[10]);
        insertNode(NodeList[10], NodeList[11]);
        insertNode(NodeList[11], NodeList[12]);
        insertNode(NodeList[12], NodeList[13]);
        insertNode(NodeList[13], NodeList[14]);
        insertNode(NodeList[14], NodeList[15]);
        drawNodes();
        createNode(40); //0
        createNode(40); //1
        createNode(40); //2
        createNode(40); //3
        createNode(40); //4
        createNode(40); //5
        createNode(40); //6
        createNode(40); //7
    });
    function createNode(r: number) {
        NodeList.push(
            new Node(
                randInt(side / 4, (3 * side) / 4),
                randInt(side / 4, (3 * side) / 4),
                r,
            ),
        );
    }
    function insertNode(head: Node, tbc: Node, len: number = 100000,show:boolean=true) {
        head.pointTo.push(tbc);
        head.pointToLen.push(len);
        head.pointConnect.push(false);
        head.pointShow.push(show);

        tbc.pointTo.push(head);
        tbc.pointToLen.push(len);
        tbc.pointConnect.push(true);
        tbc.pointShow.push(show);
        
    }
    // Utility Functions
    function randInt(min: number, max: number): number {
        return Math.round(Math.random() * (max - min) + min);
    }

    function drawNodes() {
        ctx.fillStyle = "#333333";
        ctx.fillRect(0, 0, side, side);
        ctx.fillStyle = "#F4FFF8";
        for (let i = 0; i < NodeList.length; i++) {
            ctx.fillStyle = "#F4FFF8";
            ctx.beginPath();
            ctx.arc(
                NodeList[i].x,
                NodeList[i].y,
                NodeList[i].radius,
                0,
                Math.PI * 2,
                true,
            );
            //ctx.lineWidth = 5;
            ctx.fill();
            
            for (let j = 0; j < NodeList[i].pointTo.length; j++) {
                if(NodeList[i].pointShow[j])
        {
                ctx.strokeStyle = "#F4FFF8";
                ctx.beginPath();
                ctx.moveTo(NodeList[i].x, NodeList[i].y);
                ctx.lineTo(NodeList[i].pointTo[j].x, NodeList[i].pointTo[j].y);
                ctx.lineWidth = 5;
                ctx.stroke();
                ctx.beginPath();
                ctx.fillStyle = "#333333";
                ctx.arc(
                    NodeList[i].x,
                    NodeList[i].y,
                    (3 * NodeList[i].radius) / 4,
                    0,
                    Math.PI * 2,
                    true,
                );
                //ctx.lineWidth = 5;
                ctx.fill();
                ctx.beginPath();
                ctx.fillStyle = "#333333";
                ctx.arc(
                    NodeList[i].pointTo[j].x,
                    NodeList[i].pointTo[j].y,
                    (3 * NodeList[i].radius) / 4,
                    0,
                    Math.PI * 2,
                    true,
                );
                //ctx.lineWidth = 5;
                ctx.fill();
            }}
        }
        calculatePhysics();
        requestAnimationFrame(drawNodes);
    }

    function calculatePhysics() {
        //Finding the distance between object and center

        // Calculate attraction between linked nodes
        for (let i = 0; i < NodeList.length; i++) {
            if (beingDragged != NodeList[i]) {
                let distanceFromCenterSQ =
                    (side / 2 - NodeList[i].x) ** 2 +
                    (side / 2 - NodeList[i].y) ** 2;
                if (distanceFromCenterSQ > 1000000) {
                    NodeList[i].vx +=
                        (distanceFromCenterSQ / 1000000000) *
                        (side / 2 - NodeList[i].x);
                    NodeList[i].vy +=
                        (distanceFromCenterSQ / 10000000000) *
                        (side / 2 - NodeList[i].y);
                }
                for (let j = 0; j < NodeList[i].pointTo.length; j++) {
                    let distanceSq =
                        (NodeList[i].x - NodeList[i].pointTo[j].x) ** 2 +
                        (NodeList[i].y - NodeList[i].pointTo[j].y) ** 2;
                    if (
                        distanceSq > 100000 ||
                        NodeList[i].pointToLen[j] < 50000
                    ) {
                        NodeList[i].vx +=
                            (NodeList[i].pointTo[j].radius *
                                (-NodeList[i].x + NodeList[i].pointTo[j].x)) /
                            10000;

                        NodeList[i].vy +=
                            (NodeList[i].pointTo[j].radius *
                                (-NodeList[i].y + NodeList[i].pointTo[j].y)) /
                            10000;
                    } else if (distanceSq > NodeList[i].pointToLen[j]) {
                        NodeList[i].vx +=
                            (5 *
                                (NodeList[i].pointTo[j].radius *
                                    (-NodeList[i].x +
                                        NodeList[i].pointTo[j].x))) /
                            distanceSq;

                        NodeList[i].vy +=
                            (5 *
                                (NodeList[i].pointTo[j].radius *
                                    (-NodeList[i].y +
                                        NodeList[i].pointTo[j].y))) /
                            distanceSq;
                    } else {
                        NodeList[i].vx -=
                            (1 *
                                (NodeList[i].pointTo[j].radius *
                                    (-NodeList[i].x +
                                        NodeList[i].pointTo[j].x))) /
                            distanceSq;

                        NodeList[i].vy -=
                            (1 *
                                (NodeList[i].pointTo[j].radius *
                                    (-NodeList[i].y +
                                        NodeList[i].pointTo[j].y))) /
                            distanceSq;
                    }
                }
            }
            // Apply damping to slow down the particle over time
            NodeList[i].vx *= dampingFactor;
            NodeList[i].vy *= dampingFactor;
        }

        // Calculate repulsion between particles not connected by a link
        for (let i = 0; i < NodeList.length; i++) {
            for (let j = i + 1; j < NodeList.length; j++) {
                if (!NodeList[i].pointTo.includes(NodeList[j])) {
                    let dx = NodeList[j].x - NodeList[i].x;
                    let dy = NodeList[j].y - NodeList[i].y;
                    let distanceSq = dx ** 2 + dy ** 2;

                    if (distanceSq > 0) {
                        let force = particleRepulsionConst / distanceSq;

                        let ax = (force * dx) / Math.sqrt(distanceSq);
                        let ay = (force * dy) / Math.sqrt(distanceSq);

                        NodeList[i].vx -= ax;
                        NodeList[i].vy -= ay;

                        NodeList[j].vx += ax;
                        NodeList[j].vy += ay;
                    }
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
