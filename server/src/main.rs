use actix_files::Files;
use actix_web::{
    http::{header::ContentType, StatusCode},
    get, middleware, App, HttpRequest, HttpResponse, HttpServer, Result,
};

#[get("/")]
async fn home(req: HttpRequest) -> Result<HttpResponse> {
    println!("{req:?}");
    Ok(HttpResponse::build(StatusCode::OK)
        .content_type(ContentType::html())
        .body(include_str!("../static/index.html")))
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    println!("listening on localhost:8083");
    HttpServer::new(move || {
        App::new()
            // enable automatic response compression - usually register this first
            .wrap(middleware::Compress::default())
            // enable logger - always register Actix Web Logger middleware last
            .wrap(middleware::Logger::default())
            // register simple route, handle all methods
            .service(home)
            // static files
            .service(Files::new("/static", "static").show_files_listing())
    })
    .bind(("127.0.0.1", 8083)).expect("Error binding to port")
    .run()
    .await
}
