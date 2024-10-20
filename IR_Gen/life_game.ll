; ModuleID = './life_game.c'
source_filename = "./life_game.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @start_config(i32* nocapture noundef writeonly %0, i32* nocapture noundef writeonly %1) local_unnamed_addr #0 {
  br label %4

3:                                                ; preds = %4
  ret void

4:                                                ; preds = %2, %4
  %5 = phi i32 [ 0, %2 ], [ %15, %4 ]
  %6 = tail call i32 (...) @simRand() #7
  %7 = srem i32 %6, 512
  %8 = tail call i32 (...) @simRand() #7
  %9 = srem i32 %8, 256
  %10 = shl nsw i32 %9, 9
  %11 = add nsw i32 %10, %7
  %12 = sext i32 %11 to i64
  %13 = getelementptr inbounds i32, i32* %0, i64 %12
  store i32 1, i32* %13, align 4, !tbaa !5
  %14 = getelementptr inbounds i32, i32* %1, i64 %12
  store i32 1, i32* %14, align 4, !tbaa !5
  %15 = add nuw nsw i32 %5, 1
  %16 = icmp eq i32 %15, 100000
  br i1 %16, label %3, label %4, !llvm.loop !9
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

declare i32 @simRand(...) local_unnamed_addr #2

; Function Attrs: nofree norecurse nosync nounwind readonly uwtable
define dso_local i32 @count_alive_neighbors(i32 noundef %0, i32 noundef %1, i32* nocapture noundef readonly %2) local_unnamed_addr #3 {
  %4 = add nsw i32 %0, -1
  %5 = add nsw i32 %1, -1
  %6 = icmp ugt i32 %4, 511
  br i1 %6, label %44, label %9

7:                                                ; preds = %71, %98, %102
  %8 = phi i32 [ %72, %71 ], [ %99, %98 ], [ %110, %102 ]
  ret i32 %8

9:                                                ; preds = %3
  %10 = icmp ugt i32 %5, 255
  br i1 %10, label %19, label %11

11:                                               ; preds = %9
  %12 = shl nuw nsw i32 %5, 9
  %13 = add nuw nsw i32 %12, %4
  %14 = zext i32 %13 to i64
  %15 = getelementptr inbounds i32, i32* %2, i64 %14
  %16 = load i32, i32* %15, align 4, !tbaa !5
  %17 = icmp ne i32 %16, 0
  %18 = zext i1 %17 to i32
  br label %19

19:                                               ; preds = %9, %11
  %20 = phi i32 [ 0, %9 ], [ %18, %11 ]
  %21 = icmp ugt i32 %1, 255
  br i1 %21, label %31, label %22

22:                                               ; preds = %19
  %23 = shl nuw nsw i32 %1, 9
  %24 = add nuw nsw i32 %23, %4
  %25 = zext i32 %24 to i64
  %26 = getelementptr inbounds i32, i32* %2, i64 %25
  %27 = load i32, i32* %26, align 4, !tbaa !5
  %28 = icmp ne i32 %27, 0
  %29 = zext i1 %28 to i32
  %30 = add nuw nsw i32 %20, %29
  br label %31

31:                                               ; preds = %22, %19
  %32 = phi i32 [ %20, %19 ], [ %30, %22 ]
  %33 = add nsw i32 %1, 1
  %34 = icmp ugt i32 %33, 255
  br i1 %34, label %44, label %35

35:                                               ; preds = %31
  %36 = shl nuw nsw i32 %33, 9
  %37 = add nuw nsw i32 %36, %4
  %38 = zext i32 %37 to i64
  %39 = getelementptr inbounds i32, i32* %2, i64 %38
  %40 = load i32, i32* %39, align 4, !tbaa !5
  %41 = icmp ne i32 %40, 0
  %42 = zext i1 %41 to i32
  %43 = add nuw nsw i32 %32, %42
  br label %44

44:                                               ; preds = %3, %31, %35
  %45 = phi i32 [ 0, %3 ], [ %32, %31 ], [ %43, %35 ]
  %46 = icmp ugt i32 %0, 511
  br i1 %46, label %71, label %47

47:                                               ; preds = %44
  %48 = icmp ugt i32 %5, 255
  br i1 %48, label %58, label %49

49:                                               ; preds = %47
  %50 = shl nuw nsw i32 %5, 9
  %51 = add nuw nsw i32 %50, %0
  %52 = zext i32 %51 to i64
  %53 = getelementptr inbounds i32, i32* %2, i64 %52
  %54 = load i32, i32* %53, align 4, !tbaa !5
  %55 = icmp ne i32 %54, 0
  %56 = zext i1 %55 to i32
  %57 = add nuw nsw i32 %45, %56
  br label %58

58:                                               ; preds = %49, %47
  %59 = phi i32 [ %45, %47 ], [ %57, %49 ]
  %60 = add nsw i32 %1, 1
  %61 = icmp ugt i32 %60, 255
  br i1 %61, label %71, label %62

62:                                               ; preds = %58
  %63 = shl nuw nsw i32 %60, 9
  %64 = add nuw nsw i32 %63, %0
  %65 = zext i32 %64 to i64
  %66 = getelementptr inbounds i32, i32* %2, i64 %65
  %67 = load i32, i32* %66, align 4, !tbaa !5
  %68 = icmp ne i32 %67, 0
  %69 = zext i1 %68 to i32
  %70 = add nuw nsw i32 %59, %69
  br label %71

71:                                               ; preds = %62, %58, %44
  %72 = phi i32 [ %45, %44 ], [ %59, %58 ], [ %70, %62 ]
  %73 = add nsw i32 %0, 1
  %74 = icmp ugt i32 %73, 511
  br i1 %74, label %7, label %75

75:                                               ; preds = %71
  %76 = icmp ugt i32 %5, 255
  br i1 %76, label %86, label %77

77:                                               ; preds = %75
  %78 = shl nuw nsw i32 %5, 9
  %79 = add nuw nsw i32 %78, %73
  %80 = zext i32 %79 to i64
  %81 = getelementptr inbounds i32, i32* %2, i64 %80
  %82 = load i32, i32* %81, align 4, !tbaa !5
  %83 = icmp ne i32 %82, 0
  %84 = zext i1 %83 to i32
  %85 = add nuw nsw i32 %72, %84
  br label %86

86:                                               ; preds = %75, %77
  %87 = phi i32 [ %72, %75 ], [ %85, %77 ]
  %88 = icmp ugt i32 %1, 255
  br i1 %88, label %98, label %89

89:                                               ; preds = %86
  %90 = shl nuw nsw i32 %1, 9
  %91 = add nuw nsw i32 %90, %73
  %92 = zext i32 %91 to i64
  %93 = getelementptr inbounds i32, i32* %2, i64 %92
  %94 = load i32, i32* %93, align 4, !tbaa !5
  %95 = icmp ne i32 %94, 0
  %96 = zext i1 %95 to i32
  %97 = add nuw nsw i32 %87, %96
  br label %98

98:                                               ; preds = %89, %86
  %99 = phi i32 [ %87, %86 ], [ %97, %89 ]
  %100 = add nsw i32 %1, 1
  %101 = icmp ugt i32 %100, 255
  br i1 %101, label %7, label %102

102:                                              ; preds = %98
  %103 = shl nuw nsw i32 %100, 9
  %104 = add nuw nsw i32 %103, %73
  %105 = zext i32 %104 to i64
  %106 = getelementptr inbounds i32, i32* %2, i64 %105
  %107 = load i32, i32* %106, align 4, !tbaa !5
  %108 = icmp ne i32 %107, 0
  %109 = zext i1 %108 to i32
  %110 = add nuw nsw i32 %99, %109
  br label %7
}

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @change_dead_cell(i32 noundef %0, i32 noundef %1, i32* nocapture noundef readonly %2, i32* nocapture noundef writeonly %3) local_unnamed_addr #4 {
  %5 = tail call i32 @count_alive_neighbors(i32 noundef %0, i32 noundef %1, i32* noundef %2)
  %6 = icmp eq i32 %5, 3
  %7 = shl nsw i32 %1, 9
  %8 = add nsw i32 %7, %0
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds i32, i32* %3, i64 %9
  %11 = zext i1 %6 to i32
  store i32 %11, i32* %10, align 4, !tbaa !5
  ret void
}

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @change_alive_cell(i32 noundef %0, i32 noundef %1, i32* nocapture noundef readonly %2, i32* nocapture noundef writeonly %3) local_unnamed_addr #4 {
  %5 = tail call i32 @count_alive_neighbors(i32 noundef %0, i32 noundef %1, i32* noundef %2)
  %6 = and i32 %5, -2
  %7 = icmp eq i32 %6, 2
  %8 = shl nsw i32 %1, 9
  %9 = add nsw i32 %8, %0
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds i32, i32* %3, i64 %10
  %12 = zext i1 %7 to i32
  store i32 %12, i32* %11, align 4, !tbaa !5
  ret void
}

; Function Attrs: noreturn nounwind uwtable
define dso_local void @run_game(i32* nocapture noundef %0, i32* nocapture noundef %1) local_unnamed_addr #5 {
  br label %3

3:                                                ; preds = %2, %10
  %4 = phi i32* [ %1, %2 ], [ %5, %10 ]
  %5 = phi i32* [ %0, %2 ], [ %4, %10 ]
  br label %6

6:                                                ; preds = %3, %11
  %7 = phi i64 [ 0, %3 ], [ %12, %11 ]
  %8 = trunc i64 %7 to i32
  %9 = trunc i64 %7 to i32
  br label %14

10:                                               ; preds = %11
  tail call void (...) @simFlush() #7
  br label %3

11:                                               ; preds = %29
  %12 = add nuw nsw i64 %7, 1
  %13 = icmp eq i64 %12, 512
  br i1 %13, label %10, label %6, !llvm.loop !11

14:                                               ; preds = %6, %29
  %15 = phi i64 [ 0, %6 ], [ %33, %29 ]
  %16 = shl nuw nsw i64 %15, 9
  %17 = add nuw nsw i64 %16, %7
  %18 = getelementptr inbounds i32, i32* %5, i64 %17
  %19 = load i32, i32* %18, align 4, !tbaa !5
  %20 = icmp eq i32 %19, 1
  %21 = trunc i64 %15 to i32
  br i1 %20, label %22, label %26

22:                                               ; preds = %14
  tail call void @simPutPixel(i32 noundef %9, i32 noundef %21, i32 noundef 8388736) #7
  %23 = tail call i32 @count_alive_neighbors(i32 noundef %9, i32 noundef %21, i32* noundef nonnull %5) #7
  %24 = and i32 %23, -2
  %25 = icmp eq i32 %24, 2
  br label %29

26:                                               ; preds = %14
  tail call void @simPutPixel(i32 noundef %8, i32 noundef %21, i32 noundef 16777215) #7
  %27 = tail call i32 @count_alive_neighbors(i32 noundef %8, i32 noundef %21, i32* noundef nonnull %5) #7
  %28 = icmp eq i32 %27, 3
  br label %29

29:                                               ; preds = %22, %26
  %30 = phi i1 [ %25, %22 ], [ %28, %26 ]
  %31 = zext i1 %30 to i32
  %32 = getelementptr inbounds i32, i32* %4, i64 %17
  store i32 %31, i32* %32, align 4
  %33 = add nuw nsw i64 %15, 1
  %34 = icmp eq i64 %33, 256
  br i1 %34, label %11, label %14, !llvm.loop !12
}

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #2

declare void @simFlush(...) local_unnamed_addr #2

; Function Attrs: noreturn nounwind uwtable
define dso_local void @app() local_unnamed_addr #5 {
  %1 = alloca [131072 x i32], align 16
  %2 = alloca [131072 x i32], align 16
  %3 = bitcast [131072 x i32]* %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 524288, i8* nonnull %3) #7
  call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(524288) %3, i8 0, i64 524288, i1 false)
  %4 = bitcast [131072 x i32]* %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 524288, i8* nonnull %4) #7
  call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(524288) %4, i8 0, i64 524288, i1 false)
  br label %5

5:                                                ; preds = %5, %0
  %6 = phi i32 [ 0, %0 ], [ %16, %5 ]
  %7 = tail call i32 (...) @simRand() #7
  %8 = srem i32 %7, 512
  %9 = tail call i32 (...) @simRand() #7
  %10 = srem i32 %9, 256
  %11 = shl nsw i32 %10, 9
  %12 = add nsw i32 %11, %8
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds [131072 x i32], [131072 x i32]* %1, i64 0, i64 %13
  store i32 1, i32* %14, align 4, !tbaa !5
  %15 = getelementptr inbounds [131072 x i32], [131072 x i32]* %2, i64 0, i64 %13
  store i32 1, i32* %15, align 4, !tbaa !5
  %16 = add nuw nsw i32 %6, 1
  %17 = icmp eq i32 %16, 100000
  br i1 %17, label %18, label %5, !llvm.loop !9

18:                                               ; preds = %5
  %19 = getelementptr inbounds [131072 x i32], [131072 x i32]* %2, i64 0, i64 0
  %20 = getelementptr inbounds [131072 x i32], [131072 x i32]* %1, i64 0, i64 0
  call void @run_game(i32* noundef nonnull %20, i32* noundef nonnull %19)
  unreachable
}

; Function Attrs: argmemonly mustprogress nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #6

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #2 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree norecurse nosync nounwind readonly uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree norecurse nosync nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { noreturn nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { argmemonly mustprogress nofree nounwind willreturn writeonly }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10}
!10 = !{!"llvm.loop.mustprogress"}
!11 = distinct !{!11, !10}
!12 = distinct !{!12, !10}
