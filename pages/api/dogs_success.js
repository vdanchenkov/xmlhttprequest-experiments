const message = [
  "https://images.dog.ceo/breeds/bouvier/n02106382_3425.jpg",
  "https://images.dog.ceo/breeds/retriever-flatcoated/n02099267_3660.jpg",
  "https://images.dog.ceo/breeds/leonberg/n02111129_279.jpg"
];

function wait(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

export default async (req, res) => {
  console.log("incoming body:", req.body);

  if(req.body) {
    const { timeout } = req.body;

    if(timeout > 0) {
      await wait(timeout);
    }
  }

  res.statusCode = 200;
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader("Content-Type", "application/json");
  res.end(JSON.stringify({ message, status: "success" }));
};
