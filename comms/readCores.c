
// NovusDevice.Name = "Novus Time Server"
//
// NovusDevice.BlockSize = 16
// NovusDevice.TypeInstanceReg = 0x00000000
// NovusDevice.BaseAddrLReg = 0x00000004
// NovusDevice.BaseAddrHReg = 0x00000008
// NovusDevice.IrqMaskReg = 0x0000000C
//
// var tempCore Core
//
// var tempData int64 = 0x00000000
//
// for i := int64(0); i < 256; i++ {
//
//     typeAddr := (0x00000000 + ((i * NovusDevice.BlockSize) + NovusDevice.TypeInstanceReg))
//     if readRegister(typeAddr, &tempData) == 0 {
//         if (i == 0) && ((((tempData >> int64(16)) & 0x0000FFFF) != 1) || (((tempData >> int64(0)) & 0x0000FFFF) != 1)) {
//
//             log.Println("ERROR: not a conf block at the address expected")
//             break
//
//         } else if tempData == 0 {
//             break
//
//         } else {
//
//             tempCore.CoreType = ((tempData >> 16) & 0x0000FFFF)
//             tempCore.Name = getName(tempCore.CoreType)
//             tempCore.InstanceNumber = ((tempData >> 0) & 0x0000FFFF)
//             tempCore.Registers = getRegistersByType(tempCore.CoreType)
//         }
//
//     } else {
//         log.Fatal("Error in reading modules config")
//     }
//
//     lowAddr := (0x00000000 + ((i * NovusDevice.BlockSize) + NovusDevice.BaseAddrLReg))
//     if readRegister(lowAddr, &tempData) == 0 {
//         tempCore.BaseAddrLReg = tempData
//     } else {
//         break
//     }
//
//     highAddr := (0x00000000 + ((i * NovusDevice.BlockSize) + NovusDevice.BaseAddrHReg))
//     if readRegister(highAddr, &tempData) == 0 {
//         tempCore.BaseAddrHReg = tempData
//     } else {
//         break
//     }
//
//     interruptMask := (0x00000000 + ((i * NovusDevice.BlockSize) + NovusDevice.IrqMaskReg))
//     if readRegister(interruptMask, &tempData) == 0 {
//         tempCore.IrqMaskReg = tempData
//     } else {
//         break
//     }
//
//     NovusDevice.Cores[tempCore.Name] = tempCore
//
//     //device.Cores = append(device.Cores, tempCore)
//     //fmt.Println(fmt.Sprintf("low 0x%08x", tempCore.BaseAddrLReg), fmt.Sprintf(" high 0x%08x", tempCore.BaseAddrHReg), " ", tempCore, " ", "Core type: ", get_name(tempCore.CoreType))
//
//     //if coreType == tempCore.CoreType {
//     //	fmt.Println(fmt.Sprintf("low 0x%08x", tempCore.BaseAddrLReg), fmt.Sprintf(" high 0x%08x", tempCore.BaseAddrHReg), " ", tempCore, " ", "Core type: ", get_name(tempCore.CoreType))
//     //	break
//     //} else if coreType == 0 {
//     //	fmt.Println(fmt.Sprintf("low 0x%08x", tempCore.BaseAddrLReg), fmt.Sprintf(" high 0x%08x", tempCore.BaseAddrHReg), " ", tempCore, " ", "Core type: ", get_name(tempCore.CoreType))
//     //}
//     ////coreConfig.Cores = append(coreConfig.Cores, *tempCore) ?? not sure theres a good reason for this?
//
//     // /read_core_parameters(tempCore)
// }
////fmt.Println(device)
////writeDeviceConfigFile(device)
//
////fmt.Println(NovusDevice)
// return 0
// }