import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:flutter_application_3/entity/product.dart';
import 'package:flutter_application_3/entity/variants.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:http/http.dart' as http;
import 'package:flutter_application_3/app_config.dart';

class ProductDetailScreen extends StatefulWidget {
  final Product product; // สินค้าที่จะแสดงรายละเอียด

  ProductDetailScreen({required this.product});

  @override
  _ProductDetailScreenState createState() => _ProductDetailScreenState();
}

class _ProductDetailScreenState extends State<ProductDetailScreen> {
  bool showLargeImage = false;
  @override
  void initState() {
    fetchProductVariants();

    super.initState();
  }

  List<ProductVariants> variantstList = [];
  Future<void> fetchProductVariants() async {
    final prefs = await SharedPreferences.getInstance();

    final response = await http.get(
      Uri.parse(
          "${AppConfig.SERVICE_URL}/api/product_variants/${widget.product.productId}"),
      headers: <String, String>{
        'Content-Type': 'application/json; charset=UTF-8',
        'Accept': 'application/json',
        'Authorization': 'Bearer ${prefs.getString("access_token")}',
      },
    );
    final json = jsonDecode(response.body);
    print("------------------------");

    print(json["data"]);

    List<ProductVariants> store =
        List<ProductVariants>.from(json["data"].map((item) {
      return ProductVariants.fromJSON(item);
    }));

    setState(() {
      //print(store);
      variantstList = store;
    });
  }


  Widget getProductListView() {
    return GridView.count(
      primary: false,
      crossAxisCount: 2,
      mainAxisSpacing: 20, // ปรับระยะห่างระหว่างรูปภาพตามต้องการ
      crossAxisSpacing: 20, // ปรับระยะห่างระหว่างรูปภาพตามต้องการ
      padding: const EdgeInsets.all(40),
      children: <Widget>[
        for (ProductVariants item in variantstList)
          buildProductVariantsGridItem(item),
      ],
    );
  }

  Widget buildProductVariantsGridItem(ProductVariants item) {
    return GestureDetector(
      onTap: () {},
      child: ClipRRect(
        borderRadius: BorderRadius.circular(16.0),
        child: Stack(
          alignment: Alignment.bottomLeft,
          children: [
            Container(
              padding: const EdgeInsets.all(8.0),
              color: Colors.white,
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisSize: MainAxisSize.min,
                children: [
                  Text(
                    item.colorName,
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.bold,
                      color: Colors.black,
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }


  Widget getScreen() {
    return SafeArea(child: getProductListView());
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('รายละเอียดสินค้า'),
      ),
      body: 
      SingleChildScrollView(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            GestureDetector(
              onTap: () {
                // คลิกเพื่อดูรูปภาพใหญ่
                setState(() {
                  showLargeImage = !showLargeImage;
                });
              },
              child: AnimatedContainer(
                duration: Duration(milliseconds: 500),
                curve: Curves.easeInOut,
                width: double.infinity,
                height: showLargeImage ? 400 : 200,
                child: Image.asset(
                  "assets/images/${widget.product.imgesUrl}",
                  fit: BoxFit.cover,
                ),
              ),
            ),

            Padding(
              padding: const EdgeInsets.all(16.0),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    widget.product.productName,
                    style: TextStyle(
                      fontSize: 24,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  SizedBox(height: 8.0),
                  Text(
                    "ราคา: ${widget.product.price}",
                    style: TextStyle(
                      fontSize: 18,
                      color: Colors.red,
                    ),
                  ),
                  SizedBox(height: 16.0),
                  Text(
                    "รายละเอียดสินค้า:",
                    style: TextStyle(
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                    ),
                  ),

                  // ส่วนที่จะแสดงรายละเอียดสินค้า
                  // คุณสามารถเพิ่มโค้ดเพื่อแสดงรายละเอียดสินค้าของคุณได้ที่นี่
                ],
              ),
            ),
            
            Padding(
              padding: const EdgeInsets.all(16.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: [
                  ElevatedButton(
                    onPressed: () {
                      // เพิ่มสินค้าลงในตะกร้า
                      // คุณต้องระบุสินค้าที่ถูกเลือก (widget.product) และตัวอื่น ๆ ตามที่คุณต้องการ
                    },
                    child: Text('เพิ่มลงในตะกร้า'),
                  ),
                  ElevatedButton(
                    onPressed: () {
                      // สั่งซื้อสินค้า
                      // คุณต้องระบุสินค้าที่ถูกเลือก (widget.product) และตัวอื่น ๆ ตามที่คุณต้องการ
                    },
                    child: Text('สั่งซื้อสินค้า'),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
