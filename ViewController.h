The solution involves ensuring that the `NSManagedObjectContext` remains valid throughout the asynchronous operation. We can achieve this by retaining the context before starting the asynchronous fetch and releasing it after the operation is complete.  Furthermore, we need to check if the view controller is still valid before updating the UI.  The updated code incorporates these improvements:

```objectivec
//ViewController.m
@implementation ViewController
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext;
- (void)viewDidLoad {
    [super viewDidLoad];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    // ... setup fetch request ...
    NSManagedObjectContext *context = [self.managedObjectContext copy]; //Create a copy of the context to avoid retain cycle
    [context performBlock:^{ //Asynchronous Fetch
        NSError *error = nil;
        NSArray *results = [context executeFetchRequest:fetchRequest error:&error];
        if (results) {
            dispatch_async(dispatch_get_main_queue(), ^{ // Update UI on Main Thread
                if (self) { //Check if viewController is still valid
                    [self updateUIWithResults:results];
                } else {
                    NSLog("ViewController deallocated before fetch completion.");
                }
            });
        } else {
            NSLog("Fetch Error: %@
", error);
        }
    }];
}

- (void)updateUIWithResults:(NSArray *)results {
    //Update UI with results
}
@end
```
By creating a copy of the context, we avoid creating a retain cycle and ensure the context's lifecycle is not tied directly to the view controller's.  The check `if (self)` prevents crashes by only updating the UI if the view controller still exists.