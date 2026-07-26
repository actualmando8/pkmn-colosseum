void _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID(void* camParam, s32 layerID) {
    f32 perspA;
    f32 perspB;
    f32 nearZ;
    f32 farZ;
    f32 diff[3];
    f32 mtxZ[12];
    f32 mtxY[12];
    f32 mtxT[12];
    u8* cam;
    u8* cobj;
    f32 ax;
    f32 ay;
    f32 az;

    cam = (u8*)lbl_8047AA74;
    if (cam == 0) {
        return;
    }
    if (*(u8*)(cam + 4) != 0) {
        cobj = *(u8**)(cam + 0xc);
        if (cobj == 0) {
            __assert(lbl_8047C9C4, 0x1ae, lbl_8047C9CC);
        }
        *(u32*)(cobj + 8) &= 0xFFFFFFFD;
        HSD_CObjGetPerspective(*(void**)(cam + 0xc), &perspA, &perspB);
        nearZ = HSD_CObjGetNear(*(void**)(cam + 0xc));
        farZ = HSD_CObjGetFar(*(void**)(cam + 0xc));
        fn_800D9BD0(perspA, perspB, nearZ, farZ);
        fn_800E0628(cam + 0x94, HSD_CObjGetViewingMtxPtr(*(void**)(cam + 0xc)));
        fn_800D834C();
        fn_800D7FE4(cam + 0x94);
        HSD_CObjGetEyePosition(*(void**)(cam + 0xc), cam + 0x70);
        HSD_CObjGetUpVector(*(void**)(cam + 0xc), cam + 0xf4);
        HSD_CObjGetInterest(*(void**)(cam + 0xc), cam + 0x100);
        return;
    }
    if (*(u8*)(*(u8**)(cam + 0xc) + 0x50) == 1) {
        if (*(u8*)(cam + 2) != 0) {
            if (*(u8*)(cam + 4) != 0) {
                fn_800E0628(cam + 0x94, HSD_CObjGetViewingMtxPtr(*(void**)(cam + 0xc)));
                HSD_CObjGetEyePosition(*(void**)(cam + 0xc), cam + 0x70);
                HSD_CObjGetUpVector(*(void**)(cam + 0xc), cam + 0xf4);
                HSD_CObjGetInterest(*(void**)(cam + 0xc), cam + 0x100);
            } else if (*(u8*)(cam + 1) == 1) {
                fn_800E0168(diff, cam + 0x70, cam + 0x100);
                ax = (diff[0] > lbl_8047C998) ? diff[0] : -diff[0];
                if (ax < lbl_80478ACC) {
                    ay = (diff[1] > lbl_8047C998) ? diff[1] : -diff[1];
                    if (ay < lbl_80478ACC) {
                        az = (diff[2] > lbl_8047C998) ? diff[2] : -diff[2];
                        if (az < lbl_80478ACC) {
                            *(f32*)(cam + 0x100) += lbl_8047C9A0;
                        }
                    }
                }
                fn_800E0218(cam + 0x94, cam + 0x70, cam + 0xf4, cam + 0x100);
            } else {
                GSmtxMakeXRotation(cam + 0x94, -*(f32*)(cam + 0x88));
                GSmtxMakeYRotation(mtxY, -*(f32*)(cam + 0x8c));
                GSmtxMakeZRotation(mtxZ, -*(f32*)(cam + 0x90));
                fn_800E05C0(mtxT, -*(f32*)(cam + 0x70), -*(f32*)(cam + 0x74), -*(f32*)(cam + 0x78));
                fn_800E0290(cam + 0x94, cam + 0x94, mtxY);
                fn_800E0290(cam + 0x94, cam + 0x94, mtxZ);
                fn_800E0290(cam + 0x94, cam + 0x94, mtxT);
            }
            *(u8*)(cam + 2) = 0;
        }
        cobj = *(u8**)(cam + 0xc);
        if (cobj == 0) {
            __assert(lbl_8047C9C4, 0x1a2, lbl_8047C9CC);
        }
        *(u32*)(cobj + 8) |= 0x80000002;
        PSMTXCopy(cam + 0x94, cobj + 0x54);
        HSD_CObjGetPerspective(*(void**)(cam + 0xc), &perspA, &perspB);
        nearZ = HSD_CObjGetNear(*(void**)(cam + 0xc));
        farZ = HSD_CObjGetFar(*(void**)(cam + 0xc));
        fn_800D9BD0(perspA, perspB, nearZ, farZ);
        fn_800D834C();
        fn_800D7FE4(cam + 0x94);
        return;
    }
    HSD_CObjGetOrtho(*(void**)(cam + 0xc), &perspB, &farZ, &perspA, &nearZ);
    fn_800D9B58(perspA, perspB, nearZ, farZ);
    fn_800D834C();
}
