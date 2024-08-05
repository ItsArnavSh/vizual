mod utils;
use std::cell::RefCell;
use wasm_bindgen::prelude::*;
#[wasm_bindgen]
pub struct Node {
    x: u32,
    y: u32,
    vx: u32,
    vy: u32,
    radius: u32,
}
#[wasm_bindgen]
impl Node {
    #[wasm_bindgen(constructor)]
    pub fn new(x: u32, y: u32, vx: u32, vy: u32, radius: u32) -> Node {
        Node {
            x,
            y,
            vx,
            vy,
            radius,
        }
    }
    pub fn update(&mut self) {
        self.x += self.vx;
        self.y += self.vy;
    }

    pub fn set_velocity(&mut self, vx: u32, vy: u32) {
        self.vx = vx;
        self.vy = vy;
    }
    pub fn get_x(&self) -> u32 {
        self.x
    }
    pub fn get_y(&self) -> u32 {
        self.y
    }
    pub fn get_vx(&self) -> u32 {
        self.vx
    }
    pub fn get_vy(&self) -> u32 {
        self.vy
    }
    pub fn get_radius(&self) -> u32 {
        self.radius
    }
}
thread_local! {
    static Node: RefCell<Vec<Node>> = RefCell::new(Vec::new());
}

#[wasm_bindgen]
pub fn initialize_particles(count: usize) -> *const Node {
    Node.with(|particles| {
        let mut vec = vec![];
        for _ in 0..count {
            vec.push(Node::new(0, 0, 0, 0, 5));
        }
        *particles.borrow_mut() = vec;
        particles.borrow().as_ptr()
    })
}
#[wasm_bindgen]
pub fn update_particles() {
    Node.with(|particles| {
        for particle in particles.borrow_mut().iter_mut() {
            particle.update();
        }
    });
}
