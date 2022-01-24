#import <XCTest/XCTest.h>
#import <BugsnagCocos2dx/cocoa/BugsnagCocos2dxPlugin.h>

// Fake a cocos2d library function to satisfy the linker
namespace cocos2d {
    const char* cocos2dVersion() {
        return "cocos2d-x-3.17.2";
    }
}

@interface BugsnagCocos2dxPlugin ()
+ (NSString *)parseCocos2dVersion:(const char *)rawVersion;
@end

@interface Tests : XCTestCase
@end

@implementation Tests

- (void)testParseVersionNumber {
    XCTAssertEqualObjects(@"3.17.2", [BugsnagCocos2dxPlugin parseCocos2dVersion:"cocos2d-x-3.17.2"]);
    XCTAssertEqualObjects(@"4.0", [BugsnagCocos2dxPlugin parseCocos2dVersion:"cocos2d-x-4.0"]);
    XCTAssertEqualObjects(@"3.4", [BugsnagCocos2dxPlugin parseCocos2dVersion:"cocos2d-x 3.4"]);
}

@end
