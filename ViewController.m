In Objective-C, a tricky error can occur when dealing with `NSManagedObjectContext` and asynchronous operations.  Imagine a scenario where you fetch data from a Core Data store asynchronously, and then try to update the fetched objects on the main thread.  If the asynchronous fetch completes *after* the view controller that initiated the fetch has been deallocated, you might encounter a crash. This is because you're attempting to access objects (and their context) that no longer exist.

```objectivec
//ViewController.h
@interface ViewController : UIViewController
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext;
@end

//ViewController.m
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    // ... setup fetch request ...
    NSManagedObjectContext *context = self.managedObjectContext;
    [[context performBlock:^{ //Asynchronous Fetch
        NSError *error = nil;
        NSArray *results = [context executeFetchRequest:fetchRequest error:&error];
        if (results) {
            dispatch_async(dispatch_get_main_queue(), ^{ // Update UI on Main Thread
                //If viewController is deallocated before this block executes, crash occurs
                [self updateUIWithResults:results];
            });
        } else {
            NSLog("Fetch Error: %@
", error);
        }
    }] retain]; //Retain to avoid early deallocation of context before async operation is completed
}

- (void)updateUIWithResults:(NSArray *)results {
    //Update UI with results
}
@end
```